// Fill out your copyright notice in the Description page of Project Settings.

#include "BFZombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BrainComponent.h"
#include "Components/BillboardComponent.h"
#include "BFComponents/BFSkeletalMeshComponent.h"
#include "Character/BFPlayerCharacter.h"
#include "Bot/BFZombieController.h"
#include "Components/BoxComponent.h"
#include "Character/BFPlayerController.h"

ABFZombie::ABFZombie(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer.SetDefaultSubobjectClass<UBFSkeletalMeshComponent>(ACharacter::MeshComponentName))
{
	ZombieSensingComp = ObjectInitializer.CreateDefaultSubobject<UPawnSensingComponent>(this, TEXT("Pawn sensing"));
	LeftHandDamage = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Damage|LeftHand"));
	RightHandDamage = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Damage|RightHand"));
	LeftHandDamage->SetBoxExtent(FVector(40.0f, 5.0f, 5.0f));
	RightHandDamage->SetBoxExtent(FVector(40.0f, 5.0f, 5.0f));
	BillBoardComp = ObjectInitializer.CreateDefaultSubobject<UBillboardComponent>(this, TEXT("BillBoardComp"));
	/** turn of damage detect by defaults, only when attack will turn on this collision */
	DisableLeftHandDamage();
	DisableRighthandDamage();
	LeftHandDamage->OnComponentBeginOverlap.AddDynamic(this, &ABFZombie::HandleLeftHandDamageOverlap);
	LeftHandDamage->OnComponentEndOverlap.AddDynamic(this, &ABFZombie::HandleLeftHandDamageEndOverlap);
	RightHandDamage->OnComponentBeginOverlap.AddDynamic(this, &ABFZombie::HandleRightHandDamageOverlap);
	RightHandDamage->OnComponentEndOverlap.AddDynamic(this, &ABFZombie::HandleRightHandDamageEndOverlap);
	ZombieSensingComp->OnSeePawn.AddDynamic(this, &ABFZombie::OnSeePawn);
	ZombieSensingComp->OnHearNoise.AddDynamic(this, &ABFZombie::OnHearNoise);
}

void ABFZombie::BeginPlay()
{
	Super::BeginPlay();
	LeftHandDamage->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandDamageSocket);
	RightHandDamage->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandDamageSocket);
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
		ABFPlayerController*  ActualEnemy = Character->GetPlayerController();
		if (ActualEnemy)
		{
			OnSeePlayer.Broadcast(ActualEnemy);
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
	DisableRighthandDamage();
	DisableRighthandDamage();
	Super::HandleDeath();
}

float ABFZombie::DecideHandsDamage(const FHitResult & SweepResult, ABFPlayerCharacter* PlayerCharacter, bool bFromSweep)
{
	return HandDamageBase;
}



