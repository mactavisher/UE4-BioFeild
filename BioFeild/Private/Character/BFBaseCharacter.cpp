// Fill out your copyright notice in the Description page of Project Settings.

#include "BFBaseCharacter.h"
#include "BFComponents/BFHealthComponent.h"
#include "BFComponents/BFCharacterMovementComponent.h"
#include "Weapons/BFWeaponBase.h"
#include "Projectile/BFProjectile.h"
#include "EngineMinimal.h"
#include "GameMode/BFGameMode_Evasion.h"
#include "Animation/BFAnimInstance.h"
#include "Components/BillboardComponent.h"
#include "BFComponents/BFCharacterAudioComponent.h"
#include "Character/BFPlayerController.h"

// Sets default values
ABFBaseCharacter::ABFBaseCharacter(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer.SetDefaultSubobjectClass<UBFCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	HealthComponent = ObjectInitializer.CreateDefaultSubobject<UBFHealthComponent>(this, TEXT("HealthComp"));
	CharacterVoiceComponent = ObjectInitializer.CreateDefaultSubobject<UBFCharacterAudioComponent>(this, TEXT("AudioComp"));
	CharacterBillBoardComp = ObjectInitializer.CreateDefaultSubobject<UBillboardComponent>(this, TEXT("BillBordComp"));
}

void ABFBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}


/** set components properties after all components are initialized */
void ABFBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CharacterVoiceComponent->SetOwner(this);
	CharacterVoiceComponent->SetCharacterGender(CharacterGender);
	CharacterMesh = Cast<UBFSkeletalMeshComponent>(GetMesh());
	CharacterMesh->SetCharacterOwner(this);
	CharacterVoiceComponent->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, CharacterMesh->SocketNames.MouthSocket);
	CharacterBillBoardComp->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, CharacterMesh->SocketNames.BillBoardSocket);
	CharacterState = ECharacterState::Idle;
	CharacterGender = ECharacterGender::male;
	HealthComponent->SetOwnerCharacter(this);
	CharacterMoventComp = GetBFCharacterMovement();
	CharacterMoventComp->SetOwnerCharacter(this);
	bUseControllerRotationYaw = false;
	CharacterVoiceComponent->SetOwner(this);
	bIsDead = false;
	//bind event dispatcher when components are initialized 
	HealthComponent->OnCharacterShouldDie.AddDynamic(this, &ABFBaseCharacter::HandleDeath);
	HealthComponent->OnLowHealth.AddDynamic(this, &ABFBaseCharacter::OnLowHealth);
	HealthComponent->OnHealthReduced.AddDynamic(this, &ABFBaseCharacter::OnHealthReduced);
}


/** check if character can sprint before call sprint function */
bool ABFBaseCharacter::CanSprint()
{
	return (!GetCharacterMovement()->IsFalling()) && !bIsCrouched && (CharacterHeartBeatData.bShouldCalHeartBeatRate&&CharacterHeartBeatData.bHasReachPeak);
}

void ABFBaseCharacter::SelfTurn()
{
	if (CheckCanSelfTurn())
	{
		UAnimMontage* SelectedAnim = GetSelfTurnAnim();
		if (SelectedAnim)
		{
			float AnimDuration = PlayAnimMontage(SelectedAnim, 1.f, NAME_None);
			GetWorldTimerManager().SetTimer(CharacterSelfTurnTimerHandle, this, &ABFBaseCharacter::SelfTurnFinished, 1.0f, false, AnimDuration);
			CharacterTurnData.bIsTurning = true;
		}
	}
}

void ABFBaseCharacter::StopSprint_Implementation()
{
	UBFCharacterMovementComponent* CharacterMovement = GetBFCharacterMovement();
	if (CharacterMovement)
	{
		bIsSprint = false;
		CharacterMovement->SetDefaultMaxWalkSpeed();
		CharacterHeartBeatData.bShouldRestoreHeartBeatRate = true;
	}
}

/** default sprint implementation on c++ side */
void ABFBaseCharacter::Sprint_Implementation()
{
	if (CanSprint())
	{
		UBFCharacterMovementComponent* CharacterMovement = Cast<UBFCharacterMovementComponent>(GetCharacterMovement());
		if (CharacterMovement)
		{
			//Cast<UBFAnimInstance>(GetMesh()->GetAnimInstance())->bIsSprint = true;
			bIsSprint = true;
			CharacterMovement->SetSprintSpeed();
			CharacterHeartBeatData.bShouldRestoreHeartBeatRate = false;
			if (CharacterHeartBeatData.bShouldCalHeartBeatRate)
			{
				IncreaseHeartBeatRate(0.5f);
			}
		}
	}
}

void ABFBaseCharacter::Jump()
{
	if (Super::CanJump())
	{
		Super::Jump();
	}
}

void ABFBaseCharacter::StartCrouch_Implementation()
{
	Super::Crouch(true);
}


void ABFBaseCharacter::StartDestoryCharacter()
{
	GetWorldTimerManager().SetTimer(DestoryCharacterTimerHandle, this, &ABFBaseCharacter::FinishDestroyCharacter, 1.0f, false, 10.f);
}

void ABFBaseCharacter::FinishDestroyCharacter()
{
	//	this->GetController()->UnPossess();
	GetWorldTimerManager().ClearTimer(DestoryCharacterTimerHandle);
	this->Destroy(true);
}

void ABFBaseCharacter::SetCharacterState(ECharacterState::Type NewState)
{
	this->CharacterState = NewState;
}

float ABFBaseCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	float AnimDuration = 0.5f;
	AnimDuration = Super::PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
	return AnimDuration;
}

float ABFBaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ABFGameMode_Evasion* const CurrentGameMode = GetWorld()->GetAuthGameMode<ABFGameMode_Evasion>();
	CurrentGameMode->ModifyDamage(Damage, EventInstigator, this->GetController());
	HealthComponent->ReduceHealth(Damage);
	return Damage;
}

void ABFBaseCharacter::SetHitProjectile(ABFProjectile* HitProjectile)
{
	this->LastHitProjectile = HitProjectile;
}

void ABFBaseCharacter::ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (!bIsDead)
	{
		TakeDamage(DamageTaken, DamageEvent, PawnInstigator->GetController(), DamageCauser);
	}
}

void ABFBaseCharacter::HandleDeath()
{
	CharacterVoiceComponent->PlayVoice(EVoiceType::Death);
	CharacterMesh->SetSimulatePhysics(true);
	CharacterMesh->SetComponentTickEnabled(true);
	GetBFCharacterMovement()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsDead = true;
	PrimaryActorTick.bCanEverTick = false;
	StartDestoryCharacter();
	ABFPlayerController* CharacterController = Cast<ABFPlayerController>(GetController());
	if (CharacterController)
	{
		DisableInput(CharacterController);
	}
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("I,m dead"));
#endif
}


void ABFBaseCharacter::OnHealthReduced(float ReduceAmount)
{
	CharacterVoiceComponent->PlayVoice(EVoiceType::Hurts);
}

void ABFBaseCharacter::Suicide()
{
	if (!bIsDead)
	{
		HealthComponent->SetHealth(0);
	}
}

void ABFBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	FDamageEvent FallingDamage;
	const FVector LandedVelocity = this->GetVelocity();
	StopJumping();
	ApplyDamageMomentum(DecideFallingDamage(LandedVelocity.Size()), FallingDamage, this, this);
}

float ABFBaseCharacter::DecideFallingDamage(const float VelocitySize)
{
	float Damage = 0.f;
	if (VelocitySize > 1100.f&&VelocitySize <= 1200.f)
	{
		return Damage = 10.f;
	}
	if (VelocitySize > 1200.f && VelocitySize <= 1500.f)
	{
		return Damage = 50.f;
	}
	if (VelocitySize > 1500.f)
	{
		return Damage = 100.f;
	}
	return Damage;
}

void ABFBaseCharacter::KilledBy(ABFPlayerController* Killer)
{
	if (Killer)
	{
		Killer->AddScore(20);
	}
}

void ABFBaseCharacter::IncreaseHeartBeatRate(float Value)
{
	CharacterHeartBeatData.CurrentHeartBeatRate += (Value + CharacterHeartBeatData.HeartBeatRateIncrement);
	if ((CharacterHeartBeatData.CurrentHeartBeatRate / CharacterHeartBeatData.HeartBeatRate) >= CharacterHeartBeatData.HighHeartBeatRatePercentage)
	{
		CharacterHeartBeatData.CurrentHeartBeatRate = CharacterHeartBeatData.HeartBeatRate*CharacterHeartBeatData.HighHeartBeatRatePercentage;
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("I'm overrunning!,should take some rest"));
#endif
	}
}

void ABFBaseCharacter::RestoreHeartBeat(float Value)
{
	CharacterHeartBeatData.CurrentHeartBeatRate -= Value;
}

void ABFBaseCharacter::DecreaseEnergy(float Value)
{
	CharacterEnergyData.CurrentEnergy -= (Value + CharacterEnergyData.EnergyDecrement);
	if (CharacterEnergyData.CurrentEnergy / CharacterEnergyData.Energy <= CharacterEnergyData.LowEnergyPercentage)
	{
		//do something
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, ("I'm so tired, need to eat something!"));
#endif
	}
}

void ABFBaseCharacter::Tick(float DeltaSeconds)
{
	if (CharacterHeartBeatData.bShouldCalHeartBeatRate&&CharacterHeartBeatData.bShouldRestoreHeartBeatRate)
	{
		RestoreHeartBeat(DeltaSeconds*100.f);
	}
}

void ABFBaseCharacter::CalculateTurnData()
{
	const FRotator CharacterRotation = CharacterMesh->GetForwardVector().Rotation();
	const FRotator ContorRotation = GetController()->GetControlRotation();
	const float DeltaYaw = CharacterRotation.Yaw - ContorRotation.Yaw;
	CharacterTurnData.CharacterControlYawAngle = DeltaYaw;
	if (FMath::Abs(DeltaYaw) >= 45.f)
	{
		CharacterTurnData.bShouldTurn = true;
		if (DeltaYaw >= -90.f&&DeltaYaw < -45.f)
		{
			CharacterTurnData.CharacterTurnType = ECharacterTurnType::TurnLeft45;
		}
		if (DeltaYaw <= -90.f)
		{
			CharacterTurnData.CharacterTurnType = ECharacterTurnType::TurnLeft90;
		}
		if (DeltaYaw >= 45.f&&DeltaYaw < 90.f)
		{
			CharacterTurnData.CharacterTurnType = ECharacterTurnType::TurnRight45;
		}
		if (DeltaYaw >= 90.f)
		{
			CharacterTurnData.CharacterTurnType = ECharacterTurnType::TurnRight90;
		}
	}
	else
	{
		CharacterTurnData.bShouldTurn = false;
	}
}

void ABFBaseCharacter::SelfTurnFinished()
{
	CharacterTurnData.bIsTurning = false;
}

bool ABFBaseCharacter::CheckCanSelfTurn()
{
	return CharacterTurnData.bShouldTurn && (!CharacterTurnData.bIsTurning);
}

UAnimMontage* ABFBaseCharacter::GetSelfTurnAnim()
{
	ECharacterTurnType TurnType = CharacterTurnData.CharacterTurnType;
	UAnimMontage* SelfTurnAnim = nullptr;
	switch (TurnType)
	{
	case ECharacterTurnType::TurnLeft45:SelfTurnAnim = CharacterTurnAnimData.TurnLeft45Anim; break;
	case ECharacterTurnType::TurnLeft90:SelfTurnAnim = CharacterTurnAnimData.TurnLeft90Anim; break;
	case ECharacterTurnType::TurnRight45:SelfTurnAnim = CharacterTurnAnimData.TurnRight45Anim; break;
	case ECharacterTurnType::TurnRight90:SelfTurnAnim = CharacterTurnAnimData.TurnRight90Anim; break;
	default:SelfTurnAnim = nullptr; break;
	}
	return SelfTurnAnim;
}

void ABFBaseCharacter::SetCharacterVoiceType(EVoiceType VoiceType)
{
	this->VoiceType = VoiceType;
}

bool ABFBaseCharacter::GetIsMoving()
{
	if (CharacterMoventComp)
	{
		float VelocityX = CharacterMoventComp->Velocity.X;
		float VelocityY = CharacterMoventComp->Velocity.Y;
		if (VelocityX > 0 || VelocityY > 0)
		{
			return true;
		}
		return false;
	}
	return false;
}

float ABFBaseCharacter::GetHitAngle(FVector& HitDirection)
{
	const FVector CurrentForwardVector = CharacterMesh->GetForwardVector();
	return FVector::DotProduct(CurrentForwardVector, HitDirection);
}

FVector ABFBaseCharacter::GetCharacterXYVelocity() const
{
	const FVector Velocity = GetVelocity();
	return FVector(Velocity.X, Velocity.Y, 0);
}

void ABFBaseCharacter::ReactHit()
{

}

float ABFBaseCharacter::GetHealthPercentage() const
{
	if (HealthComponent)
	{
		return HealthComponent->GetHealthPercentage();
	}
	return 0.f;
}

void ABFBaseCharacter::OnLowHealth()
{
	
}