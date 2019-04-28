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
#include "UI/BFUserWidgetBase.h"
#include "Animation/BFAnimInstance.h"
#include "Bot/BFZombie.h"
#include "GameFrameWork/Actor.h"

ABFPlayerCharacter::ABFPlayerCharacter(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer.SetDefaultSubobjectClass<UBFSkeletalMeshComponent>(ACharacter::MeshComponentName))
{
	ViewMode = EViewMode::FPS;
	CameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CameraComp"));
	Mesh3PComp=ObjectInitializer.CreateDefaultSubobject<UBFSkeletalMeshComponent>(this, TEXT("Character3pMesh"));
	Mesh3PComp->SetupAttachment(RootComponent);
	InventoryComponent = ObjectInitializer.CreateDefaultSubobject<UBFInventoryComponent>(this, TEXT("InventoryComp"));
	AimingFOVTimeLineComponent = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("AimingFOVTimelineComp"));
	NoiseEmmiterComp = ObjectInitializer.CreateDefaultSubobject<UPawnNoiseEmitterComponent>(this, TEXT("NoiseEmmiter"));
	CameraComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->AddRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight + 25.f));
	AimingFOVTimeLineComponent->SetComponentTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	bWantsToSwapWeapon = false;
	CharacterHeartBeatData.bShouldCalHeartBeatRate = true;
	CharacterActionType = ECharacterWeaponAction::Idle;
	OnUnequipWeapon.AddDynamic(this, &ABFPlayerCharacter::OnWeaponUnequiped);
	AimingFOVTimelineDelegate.BindUFunction(this, "AimingFOVDelegateCallBack");
	bUseControllerRotationPitch = false;

}

void ABFPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InventoryComponent->SetOwnerCharacter(this);
	InventoryComponent->initializeAvailableWeapon();
	CurrentAnimInstance = Cast<UBFAnimInstance>(GetBFSkeletalMesh()->GetAnimInstance());
	CharacterMesh->SetupAttachment(CameraComp);
	CharacterMesh->SetOnlyOwnerSee(true);
	Mesh3PComp->SetOwnerNoSee(true);
	Mesh3PComp->SetOnlyOwnerSee(false);
	Mesh3PComp->bOnlyOwnerSee = false;
	Mesh3PComp->bOwnerNoSee = true;
}

void ABFPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bUseControllerRotationYaw)
	{
		CalculateTurnData();
	}
	DetectItem();
	Update1pMeshTransform();
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
		CurrentWeapon = WeaponToEquip;
		float Duration = 0.5f;
		switch (ViewMode)
		{
		case EViewMode::FPS: EquipAnimMotage = CurrentWeapon->GetWeaponAnim_FPS().EquipAnim; break;
		case EViewMode::TPS: EquipAnimMotage = CurrentWeapon->GetWeaponAnim().EquipAnim; break;
		}
		Duration = PlayAnimMontage(EquipAnimMotage, 1.0f, NAME_None);
		CharacterActionType = ECharacterWeaponAction::Equiping;
		const float CameraZoffSet = WeaponToEquip->
		GetWorldTimerManager().SetTimer(EquipWeaponTimerHanle, this, &ABFPlayerCharacter::FinishEquipWeapon, 1.0f, false, Duration*0.7f);
	}
}

void ABFPlayerCharacter::FinishEquipWeapon()
{
	bIsArmed = true;
	WeaponToEquip = nullptr;
	CharacterActionType = ECharacterWeaponAction::Idle;
	bWantsToSwapWeapon = false;
	CurrentWeapon->ReciveFinishEquiping();
	CurrentAnimInstance->bIsArmed = true;
	CurrentAnimInstance->CurrentWeaponType = CurrentWeapon->GetWeaponType();
	CurrentAnimInstance->CurrentWeaponName = CurrentWeapon->GetWeaponName();
	OnCharacterArmed();
}

void ABFPlayerCharacter::UnEquipWeapon()
{
	if (CurrentWeapon)
	{
		UAnimMontage* UnEquipMontage = nullptr;
		float Duration = 0.5f;
		switch (ViewMode)
		{
		case EViewMode::FPS: UnEquipMontage = CurrentWeapon->GetWeaponAnim_FPS().EquipAnim; break;
		case EViewMode::TPS: UnEquipMontage = CurrentWeapon->GetWeaponAnim().EquipAnim; break;
		}
		Duration = PlayAnimMontage(UnEquipMontage, 1.0f, NAME_None);
		GetWorldTimerManager().SetTimer(UnequipWeaponTimerHanle, this, &ABFPlayerCharacter::FinishUnEquipWeapon, 1.0f, false, Duration*0.6);
		CharacterActionType = ECharacterWeaponAction::UnEquiping;
	}
}

void ABFPlayerCharacter::FinishUnEquipWeapon()
{
	InventoryComponent->WeaponSlots[CurrentWeapon->GetWeaponSlotIndex()].SlotWeapon = CurrentWeapon;//put the weapon back to inventory
	InventoryComponent->WeaponSlots[CurrentWeapon->GetWeaponSlotIndex()].bIsSlotOccupied = true;
	CurrentWeapon->ReceiveUnequiping();
	CurrentWeapon = nullptr;
	bIsArmed = false;
	CharacterActionType = ECharacterWeaponAction::Idle;
	OnCharacterUnArmed();
	CurrentAnimInstance->bIsArmed = false;
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
		OnCharacterUnArmed();//activate this event
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

void ABFPlayerCharacter::NotifyItemDetected(ABFInventoryItem* DetectedItem)
{
	
}

void ABFPlayerCharacter::EquipSlot1Weapon()
{
	WeaponToEquip = InventoryComponent->GiveSlotWeapon(0);
	if (CurrentWeapon&&WeaponToEquip != WeaponToEquip)
	{
		StopAnimMontage(CurrentWeapon->GetWeaponAnim().ReloadAnim);
		UnEquipWeapon();
		bWantsToSwapWeapon = true;
	}
	if (!CurrentWeapon)
	{
		EquipWeapon();
	}
}

void ABFPlayerCharacter::EquipSlot2Weapon()
{
	WeaponToEquip = InventoryComponent->GiveSlotWeapon(1);
	if (CurrentWeapon&&WeaponToEquip != WeaponToEquip)
	{
		StopAnimMontage(CurrentWeapon->GetWeaponAnim().ReloadAnim);
		UnEquipWeapon();
		bWantsToSwapWeapon = true;
	}
	if (!CurrentWeapon)
	{
		EquipWeapon();
	}
}

void ABFPlayerCharacter::EquipSlot3Weapon()
{
	WeaponToEquip = InventoryComponent->GiveSlotWeapon(2);
	if (CurrentWeapon&&WeaponToEquip != WeaponToEquip)
	{
		StopAnimMontage(CurrentWeapon->GetWeaponAnim().ReloadAnim);
		UnEquipWeapon();
		bWantsToSwapWeapon = true;
	}
	if (!CurrentWeapon)
	{
		EquipWeapon();
	}
}

void ABFPlayerCharacter::EquipSlot4Weapon()
{
	StopAnimMontage(CurrentWeapon->GetWeaponAnim().ReloadAnim);
	WeaponToEquip = InventoryComponent->GiveSlotWeapon(3);
	if (CurrentWeapon&&WeaponToEquip != WeaponToEquip)
	{
		UnEquipWeapon();
		bWantsToSwapWeapon = true;
	}
	if (!CurrentWeapon)
	{
		EquipWeapon();
	}
}

void ABFPlayerCharacter::EquipDefault()
{
	if (CurrentWeapon)
	{
		StopAnimMontage(CurrentWeapon->GetWeaponAnim().ReloadAnim);
		return;
	}
	if (InventoryComponent)
	{
		WeaponToEquip = InventoryComponent->GiveDefaultWeapon();
		if (WeaponToEquip)
		{
			EquipWeapon();
		}
	}
}

void ABFPlayerCharacter::OnWeaponUnequiped()
{
	if (bWantsToSwapWeapon)
	{
		EquipWeapon();
	}
}

void ABFPlayerCharacter::OnCharacterArmed_Implementation()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetIsSelectedAsCurrent(true);
	}
}

void ABFPlayerCharacter::OnCharacterUnArmed_Implementation()
{

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

void ABFPlayerCharacter::DetectItem()
{
	const float TraceLength = 10000.f;
	FHitResult TraceHit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(TraceHit, CameraComp->GetComponentLocation(), CameraComp->GetComponentLocation() + CameraComp->GetForwardVector()*TraceLength, ECollisionChannel::ECC_Camera);
	if (TraceHit.bBlockingHit)
	{
		DetectedItemInfo.bHitSomething = true;
		DetectedItemInfo.HitActor = TraceHit.GetActor();
		if (TraceHit.Actor!= nullptr)
		{
			DetectedItemInfo.bHitSomething = true;
			if (TraceHit.Actor->GetClass()->IsChildOf(ABFZombie::StaticClass()))
			{
				ABFZombie* HitZombie = Cast<ABFZombie>(TraceHit.GetActor());
				if (!HitZombie->GetCharacterIsDead())
				{
					DetectedItemInfo.bIsThreat = true;
				}
				else {
					DetectedItemInfo.bIsThreat = false;
				}
			}
			if (TraceHit.Actor->GetClass()->IsChildOf(ABFInventoryItem::StaticClass()))
			{
				DetectedItemInfo.HitItem = Cast<ABFInventoryItem>(TraceHit.GetActor());
				DetectedItemInfo.HitItem->ReceiveDetected(this, this, this->GetPlayerController());
				DetectedItemInfo.bHitInventoryItem = true;
				DetectedItemInfo.bHitSomething = true;
				DetectedItemInfo.bIsThreat = false;
			}
		}
		else {
			DetectedItemInfo.bHitSomething = false;
			DetectedItemInfo.HitActor = nullptr;
			DetectedItemInfo.bIsThreat = false;
			DetectedItemInfo.bHitInventoryItem = false;
			DetectedItemInfo.HitItem = nullptr;
		}
	}
	else {
		DetectedItemInfo.bHitSomething = false;
		DetectedItemInfo.HitActor = nullptr;
		DetectedItemInfo.bIsThreat = false;
		DetectedItemInfo.bHitInventoryItem = false;
		DetectedItemInfo.HitItem = nullptr;
	}
	//#if WITH_EDITOR
	//	DrawDebugLine(GetWorld(), CameraComp->GetComponentLocation(), CameraComp->GetComponentLocation()+CameraComp->GetForwardVector()*TraceLength,FColor::Green,false,0.05f);
	//#endif
}

void ABFPlayerCharacter::Update1pMeshTransform()
{
	UBFSkeletalMeshComponent* DefMesh1P = Cast<UBFSkeletalMeshComponent>(GetClass()->GetDefaultSubobjectByName(TEXT("Mesh")));
	const FMatrix DefMeshLS = FRotationTranslationMatrix(DefMesh1P->RelativeRotation, DefMesh1P->RelativeLocation);
	const FMatrix LocalToWorld = ActorToWorld().ToMatrixWithScale();
	// Mesh rotating code expect uniform scale in LocalToWorld matrix
	const FRotator RotCameraPitch(CameraComp->GetComponentRotation().Pitch, 0.0f, 0.0f);
	const FRotator RotCameraYaw(0.0f, CameraComp->GetComponentRotation().Yaw, 0.0f);
	const FMatrix LeveledCameraLS = FRotationTranslationMatrix(RotCameraYaw, CameraComp->GetComponentLocation()) * LocalToWorld.Inverse();
	const FMatrix PitchedCameraLS = FRotationMatrix(RotCameraPitch) * LeveledCameraLS;
	const FMatrix MeshRelativeToCamera = DefMeshLS * LeveledCameraLS.Inverse();
	const FMatrix PitchedMesh = MeshRelativeToCamera * PitchedCameraLS;
	CharacterMesh->SetRelativeLocationAndRotation(PitchedMesh.GetOrigin(), PitchedMesh.Rotator());
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
		UBFCharacterMovementComponent* CharacterMovement = Cast<UBFCharacterMovementComponent>(GetCharacterMovement());
		if (CharacterMovement)
		{
			bIsADS = false;
			if (bIsCrouched)
			{
				CharacterMovement->SetCrouchSpeed();
			}
			CharacterMovement->SetDefaultMaxWalkSpeed();
		}
		AimingFOVTimeLineComponent->Reverse();
	}
}

void ABFPlayerCharacter::ADS_Implementation()
{
		if (CurrentWeapon)
		{
			bUseControllerRotationYaw = true;
			CharacterMesh->GetCurrentAnimInstance()->bisADS = true;
			UBFCharacterMovementComponent* CharacterMovement = Cast<UBFCharacterMovementComponent>(GetCharacterMovement());
			CurrentWeapon->AdjustCamera();
			if (CharacterMovement)
			{
				bIsADS = true;
				if (bIsCrouched)
				{
					const float AimingSpeedModifier = CharacterMovement->AimingSpeedModifier;
					const float CrouchSpeedModifier = CharacterMovement->CrouchSpeedModifier;
					CharacterMovement->MaxWalkSpeed = CharacterMovement->GetDefaultMaxWalkSpeed()*(AimingSpeedModifier + CrouchSpeedModifier)*0.5f;
				}
				CharacterMovement->SetAimingSpeed();
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
	UBFAnimInstance* AnimInstance = Cast<UBFAnimInstance>(GetBFSkeletalMesh()->GetAnimInstance());
	AnimInstance->bCanJump = true;
}

void ABFPlayerCharacter::StopJumping()
{
	UBFAnimInstance* AnimInstance = Cast<UBFAnimInstance>(GetBFSkeletalMesh()->GetAnimInstance());
	AnimInstance->bCanJump = false;
	Super::StopJumping();
}

