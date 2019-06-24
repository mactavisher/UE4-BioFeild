// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Sound/SoundCue.h"
#include "Animation/BFAnimInstance.h"
#include "Weapons/BFWeaponBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "BFComponents/BFSkeletalMeshComponent.h"
#include "BFComponents/BFCharacterMovementComponent.h"
#include "BFComponents/BFInventoryComponent.h"
#include "BFComponents/BFWeaponMeshComponent.h"
#include "EngineMinimal.h"
#include "Animation/BFAnimInstance.h"
#include "Bot/BFZombie.h"
#include "UI/BFCharacterWidget.h"
#include "UI/BFCurrentWeaponWidget.h"
#include "Attachments/BFAttachment_Scope.h"
#include "GameFrameWork/Actor.h"

ABFPlayerCharacter::ABFPlayerCharacter(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer.SetDefaultSubobjectClass<UBFSkeletalMeshComponent>(ACharacter::MeshComponentName))
{
	ViewMode = EViewMode::FPS;
	SpringArmComp = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("SpringArm"));
	CameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CameraComp"));
	Mesh3PComp = ObjectInitializer.CreateDefaultSubobject<UBFSkeletalMeshComponent>(this, TEXT("Character3pMesh"));
	InventoryComponent = ObjectInitializer.CreateDefaultSubobject<UBFInventoryComponent>(this, TEXT("InventoryComp"));
	GetCapsuleComponent()->SetCapsuleRadius(GetCapsuleComponent()->GetScaledCapsuleRadius()*1.5f);
	AimingFOVTimeLineComponent = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("AimingFOVTimelineComp"));
	NoiseEmmiterComp = ObjectInitializer.CreateDefaultSubobject<UPawnNoiseEmitterComponent>(this, TEXT("NoiseEmmiter"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 2.f;
	CameraComp->SetupAttachment(SpringArmComp);
	Mesh3PComp->SetupAttachment(GetCapsuleComponent());
	AimingFOVTimeLineComponent->SetComponentTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	bWantsToSwapWeapon = false;
	CharacterHeartBeatData.bShouldCalHeartBeatRate = true;
	CharacterActionType = ECharacterWeaponAction::Idle;
	OnUnequipWeapon.AddDynamic(this, &ABFPlayerCharacter::OnWeaponUnequiped);
	AimingFOVTimelineDelegate.BindUFunction(this, "AimingFOVDelegateCallBack");
	bUseControllerRotationPitch = false;
	bReplicates = true;
	bReplicateMovement = true;
}

void ABFPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CameraOriginalRelativeTransform = CameraComp->GetRelativeTransform();
	InventoryComponent->SetOwnerCharacter(this);
	InventoryComponent->initializeAvailableWeapon();
	CurrentAnimInstance = Cast<UBFAnimInstance>(GetBFSkeletalMesh()->GetAnimInstance());
	CharacterMesh->AttachToComponent(SpringArmComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CharacterMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	CharacterMesh->SetOnlyOwnerSee(true);
	Mesh3PComp->SetOwnerNoSee(true);
	Mesh3PComp->SetOnlyOwnerSee(false);
	Mesh3PComp->bOnlyOwnerSee = false;
	Mesh3PComp->bOwnerNoSee = true;
	//add rotation to make 1p mesh look forward
	CharacterMesh->AddRelativeRotation(FRotator(0.f, -90.f, 0.f));
	//add relative location to match 3p mesh bone location for later use purpose
	CharacterMesh->AddRelativeLocation(FVector(18.f, 0.f, -150.f));
	// adjust camera location to match weapon action such as ADS,this should be a valuable get from from weapon property
	CameraComp->AddRelativeLocation(FVector(5.f, 0.f, 20.25f));
	CreateCharacterWidgetInstance();
}

void ABFPlayerCharacter::ResetTargetHitInfo()
{
	TargetHitInfo.bIsTargetDead = false;
	TargetHitInfo.DamgeCause = 0.f;
	TargetHitInfo.Victim = nullptr;
}

void ABFPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bUseControllerRotationYaw)
	{
		CalculateTurnData();
	}
	//AimingFOVTimeLineComponent->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}

void ABFPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UBFSkeletalMeshComponent* Mesh = Cast<UBFSkeletalMeshComponent>(GetMesh());
	Cast<UBFSkeletalMeshComponent>(GetMesh())->OnCanStep.AddDynamic(this, &ABFPlayerCharacter::HandleStepOn);
	bUseControllerRotationYaw = true;
	bIsArmed = false;
	RegisterAllComponents();
}

/** Player input bindings */
void ABFPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABFPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABFPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABFPlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABFPlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABFPlayerCharacter::StopSprint);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABFPlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABFPlayerCharacter::StopFireWeapon);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABFPlayerCharacter::AimingDispacher);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABFPlayerCharacter::StopAimingDispacher);
	PlayerInputComponent->BindAction("Reload", IE_Released, this, &ABFPlayerCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction("EquipDefault", IE_Pressed, this, &ABFPlayerCharacter::EquipDefault);
	PlayerInputComponent->BindAction("EquipSlot1", IE_Pressed, this, &ABFPlayerCharacter::EquipSlot1Weapon);
	PlayerInputComponent->BindAction("EquipSlot2", IE_Pressed, this, &ABFPlayerCharacter::EquipSlot2Weapon);
	PlayerInputComponent->BindAction("EquipSlot3", IE_Pressed, this, &ABFPlayerCharacter::EquipSlot3Weapon);
	PlayerInputComponent->BindAction("EquipSlot4", IE_Pressed, this, &ABFPlayerCharacter::EquipSlot4Weapon);
	PlayerInputComponent->BindAction("Suicide", IE_Pressed, this, &ABFPlayerCharacter::Suicide);
	PlayerInputComponent->BindAction("ToggleFireMode", IE_Pressed, this, &ABFPlayerCharacter::ToggleFireMode);
	PlayerInputComponent->BindAction("ToggleAimMode", IE_Pressed, this, &ABFPlayerCharacter::ToggleAimMode);
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &ABFPlayerCharacter::DropCurrentWeapon);
	PlayerInputComponent->BindAction("CameraStyle", IE_Pressed, this, &ABFPlayerCharacter::FreeStyleCamera);
	PlayerInputComponent->BindAction("CameraStyle", IE_Pressed, this, &ABFPlayerCharacter::FixedStyleCamera);
}

void ABFPlayerCharacter::EquipWeapon()
{
	if (CheckCanEquip())
	{
		UAnimMontage* EquipAnimMotage = nullptr;
		CurrentAnimInstance->bIsArmed = true;
		CurrentWeapon = WeaponToEquip;
		CurrentWeapon->OnWeaponEquiping();
		float Duration = 0.5f;
		bIsArmed = true;
		CurrentAnimInstance->CurrentWeaponType = CurrentWeapon->GetWeaponType();
		CurrentAnimInstance->CurrentWeaponName = CurrentWeapon->GetWeaponName();
		switch (ViewMode)
		{
		case EViewMode::FPS: EquipAnimMotage = CurrentWeapon->GetWeaponAnim_FPS().EquipAnim; break;
		case EViewMode::TPS: EquipAnimMotage = CurrentWeapon->GetWeaponAnim().EquipAnim; break;
		}
		Duration = PlayAnimMontage(EquipAnimMotage, 1.0f, NAME_None);
		CharacterActionType = ECharacterWeaponAction::Equiping;
		//const FVector  CameraAdjustVector = WeaponToEquip->GetADSCameraAdjustVector();
		//CameraComp->AddRelativeLocation(CameraAdjustVector);
		GetWorldTimerManager().SetTimer(EquipWeaponTimerHanle, this, &ABFPlayerCharacter::FinishEquipWeapon, 1.0f, false, Duration);
	}
}

void ABFPlayerCharacter::FinishEquipWeapon()
{
	CurrentWeapon->OnWeaponEquipingFinished();
	CharacterActionType = ECharacterWeaponAction::Idle;
	bWantsToSwapWeapon = false;

}

void ABFPlayerCharacter::UnEquipWeapon()
{
	if (CurrentWeapon)
	{
		UAnimMontage* UnEquipMontage = nullptr;
		float Duration = 0.5f;
		CurrentAnimInstance->bIsArmed = false;
		CurrentWeapon->OnWeaponUnEquiping();
		UnEquipMontage = CurrentWeapon->GetWeaponAnim_FPS().UnEquipAnim;
		Duration = PlayAnimMontage(UnEquipMontage, 1.0f, NAME_None);
		GetWorldTimerManager().SetTimer(UnequipWeaponTimerHanle, this, &ABFPlayerCharacter::FinishUnEquipWeapon, 1.0f, false, Duration);
		CharacterActionType = ECharacterWeaponAction::UnEquiping;
	}
}

void ABFPlayerCharacter::FinishUnEquipWeapon()
{
	CurrentWeapon->OnWeaponUnequipingFinished();
	CurrentWeapon = nullptr;
	bIsArmed = false;
	CharacterActionType = ECharacterWeaponAction::Idle;
	OnUnequipWeapon.Broadcast();
	if (bWantsToSwapWeapon&&WeaponToEquip)
	{
		EquipWeapon();
	}
}

void ABFPlayerCharacter::AimingDispacher()
{
	if (CurrentWeapon)
	{
		ADS();
	}
}

void ABFPlayerCharacter::StopAimingDispacher()
{
	if (CurrentWeapon)
	{
		StopADS();
	}
}

void ABFPlayerCharacter::ReloadWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ReloadWeapon();
	}
}


void ABFPlayerCharacter::AttatchWeaponToHand()
{
	if (WeaponToEquip)
	{
		WeaponToEquip->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, CharacterMesh->SocketNames.CurrentWeaponRifleSocket);
		WeaponToEquip->WeaponMeshComponent->SetHiddenInGame(false);
	}
}

void ABFPlayerCharacter::DetachCurrentWeaponFromHand()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->WeaponMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		//CurrentWeapon->WeaponMeshComponent->SetSimulatePhysics(true);
	}
}

void ABFPlayerCharacter::ToggleFireMode()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ToggleFireMode();
	}
}

void ABFPlayerCharacter::DropCurrentWeapon()
{
	if (CurrentWeapon)
	{
		DetachCurrentWeaponFromHand();
		CurrentWeapon->WeaponMeshComponent->SetSimulatePhysics(true);
		const FVector MeshLocation = CurrentWeapon->WeaponMeshComponent->GetComponentLocation();
		CurrentWeapon->WeaponMeshComponent->AddImpulseAtLocation(MeshLocation.ForwardVector*100.f, MeshLocation);
		CurrentWeapon->SetWeaponOwner(nullptr);
		CurrentWeapon->HandleDroped();
		InventoryComponent->RemoveSlotWeapon(CurrentWeapon->GetWeaponSlotIndex());
	}
}

void ABFPlayerCharacter::PickUpWeapon()
{
	if (CurrentWeapon)
	{
		DropCurrentWeapon();
	}
	//@To do this should be bind with events
}


bool ABFPlayerCharacter::CheckCanEquip()
{
	if (CharacterActionType == ECharacterWeaponAction::Equiping || CharacterActionType == ECharacterWeaponAction::UnEquiping || WeaponToEquip == nullptr)
	{
		return false;
	}
	return true;
}

void ABFPlayerCharacter::StartHoldBreath()
{

}

void ABFPlayerCharacter::StopHoldBreath()
{

}

void ABFPlayerCharacter::ToggleAimMode()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ToggleAimMode();
	}
}

void ABFPlayerCharacter::CreateCharacterWidgetInstance()
{
		if (CharacterWidgetClass)
		{
			CharacterWidgetInstance = CreateWidget<UBFCharacterWidget>(GetPlayerController(), CharacterWidgetClass);
			CharacterWidgetInstance->AddToPlayerScreen(0);
		}
		if (CurrenetWeaponWidgetClass)
		{
			CurrentWeaponWidgetInstance = CreateWidget<UBFCurrentWeaponWidget>(GetPlayerController(), CurrenetWeaponWidgetClass);
			CurrentWeaponWidgetInstance->AddToPlayerScreen(0);
		}
}

void ABFPlayerCharacter::Sprint()
{
	Super::Sprint();
	CurrentAnimInstance->bIsSprinting = true;
}

void ABFPlayerCharacter::StopSprint()
{
	Super::StopSprint();
	CurrentAnimInstance->bIsSprinting = false;
}

void ABFPlayerCharacter::NotifyItemDetected(ABFInventoryItem* DetectedItem)
{

}

void ABFPlayerCharacter::EquipSlot1Weapon()
{
	EquipSlotWeapon(0);
}

void ABFPlayerCharacter::EquipSlot2Weapon()
{
	EquipSlotWeapon(1);
}

void ABFPlayerCharacter::EquipSlot3Weapon()
{
	EquipSlotWeapon(2);
}

void ABFPlayerCharacter::EquipSlot4Weapon()
{
	EquipSlotWeapon(3);
}

void ABFPlayerCharacter::EquipDefault()
{
	EquipSlotWeapon(255);
}

void ABFPlayerCharacter::OnWeaponUnequiped()
{
	if (bWantsToSwapWeapon)
	{
		EquipWeapon();
	}
}

void ABFPlayerCharacter::SetPlayerController(ABFPlayerController* NewController)
{
	this->PlayerController = NewController;
}

bool ABFPlayerCharacter::GetCanEnterSprint()
{
	if (bIsSprint)
	{
		if (!CharacterMoventComp->IsFalling())
		{
			return true;
		}
	}
	return false;
}

void ABFPlayerCharacter::HandleStepOn(FHitResult Hit1, FHitResult Hit2)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("I want to step on"));
}

void ABFPlayerCharacter::FreeStyleCamera()
{
	CharacterTurnData.bShouldTurn = false;
	bUseControllerRotationYaw = false;
}

void ABFPlayerCharacter::FixedStyleCamera()
{
	CharacterTurnData.bShouldTurn = true;
}

void ABFPlayerCharacter::InitializeUserWidget()
{
	//create inventory widget
	if (InventoryComponent->InventoryWidgetClass)
	{
		InventoryComponent->SetInventoryWidget(CreateWidget<UBFUserWidgetBase>(GetPlayerController(), InventoryComponent->InventoryWidgetClass));
		InventoryComponent->GetInventoryWidget()->AddToViewport(0);
	}
}

void ABFPlayerCharacter::ReceiveHitTarget(float DamageAmount, bool VictimDead, class ABFBaseCharacter* Victim)
{
	TargetHitInfo.bIsTargetDead = VictimDead;
	TargetHitInfo.DamgeCause = DamageAmount;
	Victim = Victim;
	GetWorldTimerManager().SetTimer(ResetTargetHitInfoTimerHandle, this, &ABFPlayerCharacter::ResetTargetHitInfo, 1.0f, false, 0.15f);
}

void ABFPlayerCharacter::StopCurrentPlayingMontage()
{
	//from the source code we know ,if montage is null ,then it will stop all the montages which is not what we want
	const UAnimMontage* CurrentPlayingMontage = CurrentAnimInstance->GetCurrentActiveMontage();
	if (CurrentPlayingMontage)
	{
		CurrentAnimInstance->Montage_Stop(0.1f, CurrentPlayingMontage);
	}
}

void ABFPlayerCharacter::EquipSlotWeapon(uint8 WeaponSlotIndex)
{
	if (WeaponSlotIndex == 255)
	{
		WeaponToEquip = InventoryComponent->GiveDefaultWeapon();
	}
	else if (WeaponSlotIndex != 255)
	{
		WeaponToEquip = InventoryComponent->GiveSlotWeapon(WeaponSlotIndex);
	}
	if (!CurrentWeapon)
	{
		EquipWeapon();
	}
	else if (CurrentWeapon&&CurrentWeapon->GetWeaponSlotIndex() == WeaponSlotIndex)
	{
		UnEquipWeapon();
		bWantsToSwapWeapon = false;
	}
	else if (CurrentWeapon&&CurrentWeapon->GetWeaponSlotIndex() != WeaponSlotIndex)
	{
		UnEquipWeapon();
		bWantsToSwapWeapon = true;
	}
}

void ABFPlayerCharacter::Update1pMeshTransform(const FVector& CameraLocation, const FRotator& CameraRotation)
{
	if (CharacterMesh)
	{
		UBFSkeletalMeshComponent* DefMesh1P = CharacterMesh;
		const FMatrix DefMeshLS = FRotationTranslationMatrix(DefMesh1P->RelativeRotation, DefMesh1P->RelativeLocation);
		const FMatrix LocalToWorld = ActorToWorld().ToMatrixWithScale();

		// Mesh rotating code expect uniform scale in LocalToWorld matrix

		const FRotator RotCameraPitch(CameraRotation.Pitch, 0.0f, 0.0f);
		const FRotator RotCameraYaw(0.0f, CameraRotation.Yaw, 0.0f);

		const FMatrix LeveledCameraLS = FRotationTranslationMatrix(RotCameraYaw, CameraLocation) * LocalToWorld.Inverse();
		const FMatrix PitchedCameraLS = FRotationMatrix(RotCameraPitch) * LeveledCameraLS;
		const FMatrix MeshRelativeToCamera = DefMeshLS * LeveledCameraLS.Inverse();
		const FMatrix PitchedMesh = MeshRelativeToCamera * PitchedCameraLS;

		CharacterMesh->SetRelativeLocationAndRotation(PitchedMesh.GetOrigin(), PitchedMesh.Rotator());
	}
}

void ABFPlayerCharacter::AimingFOVDelegateCallBack()
{
	float TimelinePosition = AimingFOVTimeLineComponent->GetPlaybackPosition();
	if (AimingFOVCurve)
	{
		CameraComp->SetFieldOfView(AimingFOVCurve->GetFloatValue(TimelinePosition));
	}
}

void ABFPlayerCharacter::StopFireWeapon_Implementation()
{
	if (CurrentWeapon)
	{
		//	CurrentWeapon->BurstShotFinished();
		CurrentWeapon->StopFire();
	}
}

void ABFPlayerCharacter::FireWeapon_Implementation()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void ABFPlayerCharacter::StopADS_Implementation()
{
	if (CurrentWeapon)
	{
		bUseControllerRotationYaw = true;
		CharacterMesh->GetCurrentAnimInstance()->bisADS = false;
		CurrentWeapon->ResetAdjustedCamera();
		ABFAttachment_Scope* CurrentScope = Cast<ABFAttachment_Scope>(CurrentWeapon->ScopeSlot.AttachmentInstance);
		if (CurrentScope)
		{
			CurrentScope->OnStopADS();
		}
		const FVector CameraAdjustVector = CurrentWeapon->GetADSCameraAdjustVector();
		FVector BacktoVector(-CameraAdjustVector.X, -CameraAdjustVector.Y, -CameraAdjustVector.Z);
		CameraComp->AddRelativeLocation(BacktoVector);
		UBFCharacterMovementComponent* CharacterMovement = Cast<UBFCharacterMovementComponent>(GetCharacterMovement());
		if (CharacterMovement)
		{
			bIsADS = false;
			if (bIsCrouched)
			{
				CharacterMovement->SetCrouch();
			}
			CharacterMovement->SetDefault();
		}
		AimingFOVTimeLineComponent->Reverse();
	}
}

void ABFPlayerCharacter::ADS_Implementation()
{
	if (CurrentWeapon)
	{
		bUseControllerRotationYaw = true;
		ABFAttachment_Scope* CurrentScope = Cast<ABFAttachment_Scope>(CurrentWeapon->ScopeSlot.AttachmentInstance);
		if (CurrentScope)
		{
			CurrentScope->OnADS();
		}
		CharacterMesh->GetCurrentAnimInstance()->bisADS = true;
		UBFCharacterMovementComponent* CharacterMovement = Cast<UBFCharacterMovementComponent>(GetCharacterMovement());
		CurrentWeapon->AdjustCamera();
		const FVector ADSAdjustVector = CurrentWeapon->GetADSCameraAdjustVector();
		CameraComp->AddRelativeLocation(ADSAdjustVector);
		if (CharacterMovement)
		{
			bIsADS = true;
			if (bIsCrouched)
			{
				const float AimingSpeedModifier = CharacterMovement->AimingSpeedModifier;
				const float CrouchSpeedModifier = CharacterMovement->CrouchSpeedModifier;
				CharacterMovement->MaxWalkSpeed = CharacterMovement->GetDefaultMaxWalkSpeed()*(AimingSpeedModifier + CrouchSpeedModifier)*0.5f;
			}
			CharacterMovement->SetAiming();
		}
		if (AimingFOVTimeLineComponent)
		{
			AimingFOVTimeLineComponent->AddInterpFloat(AimingFOVCurve, AimingFOVTimelineDelegate);
			AimingFOVTimeLineComponent->Play();
		}
	}
}

void ABFPlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABFPlayerCharacter::MoveForward(float Value)
{
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ABFPlayerCharacter::Jump()
{
	Super::Jump();
	CurrentAnimInstance->bCanJump = true;
}

void ABFPlayerCharacter::StopJumping()
{
	CurrentAnimInstance->bCanJump = false;
	Super::StopJumping();
}

