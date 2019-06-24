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
#include "Animation/BFZombieAnimation.h"
#include "Components/BoxComponent.h"
#include "Character/BFPlayerController.h"
#include "Character/BFPlayerCharacter.h"

ABFZombie::ABFZombie(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer.SetDefaultSubobjectClass<UBFSkeletalMeshComponent>(ACharacter::MeshComponentName))
{
	ZombieSensingComp = ObjectInitializer.CreateDefaultSubobject<UPawnSensingComponent>(this, TEXT("Pawn sensing"));
	MoveType = EZombieMoveType::WalkTo;
	FScriptDelegate LeftHandDamageDelegate;
	FScriptDelegate RightHandDamageDelegate;
	RightHandDamageDelegate.BindUFunction(this, "HandleRightHandDamageOverlap");
	LeftHandDamageDelegate.BindUFunction(this, "HandleLeftHandDamageOverlap");
	ZombieSensingComp->OnSeePawn.AddDynamic(this, &ABFZombie::OnSeePawn);
	ZombieSensingComp->OnHearNoise.AddDynamic(this, &ABFZombie::OnHearNoise);
	PrimaryActorTick.bCanEverTick = true;
	HandDamageBase = 20.f;
	bUseControllerRotationYaw = false;
}

void ABFZombie::BeginPlay()
{
	Super::BeginPlay();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	GetBFCharacterMovement()->bUseRVOAvoidance = true;
	GetBFCharacterMovement()->AvoidanceConsiderationRadius = 300.f;
}

void ABFZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

float ABFZombie::PlayAttackingAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	const int32 RamdonNum = FMath::RandRange(0, 2);
	if (RamdonNum == 0)
	{
		StartSectionName = "LeftHandAttack";
	}
	if (RamdonNum == 1)
	{
		StartSectionName = "RightHandAttack";
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


void ABFZombie::HandleDeath()
{
	Super::HandleDeath();
	uint8 RandomNumber = FMath::RandHelper(7);
	if (RandomNumber == 2||RandomNumber==0)
	{
		CharacterMesh->SetSimulatePhysics(false);
		Cast<UBFZombieAnimation>(CharacterMesh->GetAnimInstance())->bIsDead = true;
	}
	ZombieSensingComp->Deactivate();
}


