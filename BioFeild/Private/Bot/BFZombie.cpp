// Fill out your copyright notice in the Description page of Project Settings.

#include "BFZombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BrainComponent.h"
#include "Components/BillboardComponent.h"
#include "BFComponents/BFSkeletalMeshComponent.h"
#include "Character/BFPlayerCharacter.h"
#include "Bot/BFZombieController.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "Character/BFPlayerController.h"
#include "Character/BFPlayerCharacter.h"

ABFZombie::ABFZombie(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer.SetDefaultSubobjectClass<UBFSkeletalMeshComponent>(ACharacter::MeshComponentName))
{
	ZombieSensingComp = ObjectInitializer.CreateDefaultSubobject<UPawnSensingComponent>(this, TEXT("Pawn sensing"));
	LeftHandDamage = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Damage|LeftHand"));
	RightHandDamage = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Damage|RightHand"));
	LeftHandDamage->SetBoxExtent(FVector(40.0f, 5.0f, 5.0f));
	RightHandDamage->SetBoxExtent(FVector(40.0f, 5.0f, 5.0f));
	/** turn of damage detect by defaults, only when attack will turn on this collision */
    DisableLeftHandDamage();
    DisableRighthandDamage();
	FScriptDelegate LeftHandDamageDelegate;
	FScriptDelegate RightHandDamageDelegate;
	RightHandDamageDelegate.BindUFunction(this, "HandleRightHandDamageOverlap");
	LeftHandDamageDelegate.BindUFunction(this, "HandleLeftHandDamageOverlap");
	LeftHandDamage->OnComponentBeginOverlap.AddUnique(LeftHandDamageDelegate);
	//LeftHandDamage->OnComponentEndOverlap.AddDynamic(this, &ABFZombie::HandleLeftHandDamageEndOverlap);
	RightHandDamage->OnComponentBeginOverlap.AddUnique(LeftHandDamageDelegate);
	//RightHandDamage->OnComponentEndOverlap.AddDynamic(this, &ABFZombie::HandleRightHandDamageEndOverlap);
	ZombieSensingComp->OnSeePawn.AddDynamic(this, &ABFZombie::OnSeePawn);
	ZombieSensingComp->OnHearNoise.AddDynamic(this, &ABFZombie::OnHearNoise);
	PrimaryActorTick.bCanEverTick = true;
}

void ABFZombie::BeginPlay()
{
	Super::BeginPlay();
	LeftHandDamage->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandDamageSocket);
	RightHandDamage->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandDamageSocket);
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, 100.f);
}

void ABFZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LeftHandDamage->SetupAttachment(CharacterMesh, LeftHandDamageSocket);
	RightHandDamage->SetupAttachment(CharacterMesh, RightHandDamageSocket);
}

float ABFZombie::PlayAttackingAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	const int32 RamdonNum = FMath::RandRange(0, 2);
	if (RamdonNum == 0)
	{
		StartSectionName = "LeftHandAttack";
		EnableLeftHandDamage();
	}
	if (RamdonNum == 1)
	{
		StartSectionName = "RightHandAttack";
		EnableRighthandDamage();
	}
	else {
		StartSectionName = "Name_None";
	}
	float Duration = PlayAnimMontage(AttackAnim, 1.0f, StartSectionName);
	return Duration;
}

void ABFZombie::OnSeePawn(APawn* Pawn)
{
	const ABFPlayerCharacter* const Character = Cast<ABFPlayerCharacter>(Pawn);
	if (Character)
	{
		ABFPlayerController*  PlayerController = Character->GetPlayerController();
		if (PlayerController)
		{
			ABFPlayerCharacter* PlayerPawn = PlayerController->GetPoccessedPlayerCharacter();
			const FVector PlayerLocation = PlayerPawn->GetActorLocation();
			OnSeePlayer.Broadcast(PlayerController,PlayerPawn,PlayerLocation);
		}
	}
}

void ABFZombie::OnHearNoise(APawn* Instigator, const FVector& Location, float Volume)
{
	ABFPlayerCharacter*  Character = Cast<ABFPlayerCharacter>(Instigator);
	const ABFPlayerController* const ActualEnemy = Character->GetPlayerController();
	if (ActualEnemy)
	{
		OnHearPlayer.Broadcast(Character, Location, Volume);
	}
}

void ABFZombie::EnableLeftHandDamage()
{
	LeftHandDamage->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABFZombie::EnableRighthandDamage()
{
	RightHandDamage->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABFZombie::DisableLeftHandDamage()
{
	LeftHandDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABFZombie::DisableRighthandDamage()
{
	RightHandDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABFZombie::HandleLeftHandDamageOverlap(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("LeftDamage"));
	ABFPlayerCharacter* PlayerCharacter = Cast<ABFPlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		const float DamageTaken = PlayerCharacter->TakeDamage(DecideHandsDamage(SweepResult, PlayerCharacter, bFromSweep), ZombieAttackDamageEvent, GetZombieController(), this);
		if (DamageTaken >= 0)
		{
			OnDamagePlayer.Broadcast(this, DamageTaken, SweepResult);
		}
	}
	
}

void ABFZombie::HandleRightHandDamageOverlap(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("RightDamage"));
	ABFPlayerCharacter* PlayerCharacter = Cast<ABFPlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		const float DamageTaken = PlayerCharacter->TakeDamage(DecideHandsDamage(SweepResult, PlayerCharacter, bFromSweep), ZombieAttackDamageEvent, GetZombieController(), this);
		if (DamageTaken >= 0)
		{
			OnDamagePlayer.Broadcast(this, DamageTaken, SweepResult);
		}
	}
}

void ABFZombie::HandleLeftHandDamageEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DisableLeftHandDamage();
	bIsAttacking = false;
}

void ABFZombie::HandleRightHandDamageEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DisableRighthandDamage();
	bIsAttacking = false;
}


void ABFZombie::HandleDeath()
{
	Super::HandleDeath();
	DisableRighthandDamage();
	DisableRighthandDamage();
	ZombieSensingComp->Deactivate();
}

float ABFZombie::DecideHandsDamage(const FHitResult & SweepResult, ABFPlayerCharacter* PlayerCharacter, bool bFromSweep)
{
	return HandDamageBase;
}



