// Fill out your copyright notice in the Description page of Project Settings.

#include "BFGameMode_Evasion.h"
#include "Bot/BFZombie.h"
#include "Engine/World.h"
#include "Bot/BFZombieController.h"
#include "Pickups/BFPickup_Ammo.h"
#include "Pickups/BFPickup_Health.h"
#include "TimerManager.h"

ABFGameMode_Evasion::ABFGameMode_Evasion(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	
}

void ABFGameMode_Evasion::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	bAllowTeamDamage = false;
	GameDifficulty = EGameDifficulty::easy;
	CurrentGameLevel = EGameLevel::SurviveTheSurge;
	MaximunLives = 10;
	CurrentLivesLeft = MaximunLives;
	TimeBetweenWave = 20.f;
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABFGameMode_Evasion::Prepare()
{
	GetWorldTimerManager().SetTimer(GameStartTimerHandle, this, &ABFGameMode_Evasion::FinishPrePare, 1.f, false, PrepareTime);
}

void ABFGameMode_Evasion::FinishPrePare()
{
	GetWorldTimerManager().ClearTimer(GameStartTimerHandle);
	TimeBetweenWave = 10.f;
}

void ABFGameMode_Evasion::FinishCurrentWave()
{

}


void ABFGameMode_Evasion::ModifyDamage(float& Outdamage, AController* DamageCauser, AController*DamagedPlayer, FDamageEvent DamageEvent)
{
	Super::ModifyDamage(Outdamage, DamageCauser, DamagedPlayer, DamageEvent);
}

void ABFGameMode_Evasion::ResetCurrentLivesLeft()
{
	CurrentLivesLeft = MaximunLives;
}

void ABFGameMode_Evasion::BeginPlay()
{

}

void ABFGameMode_Evasion::StartMatch()
{
	Super::StartMatch();
}

void ABFGameMode_Evasion::EndMatch()
{
	Super::EndMatch();
}

void ABFGameMode_Evasion::SpawnPickups(ABFZombieController* RequestController)
{
	FActorSpawnParameters PickupsSpawnParameters;
	PickupsSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	int32 RamdonNum = FMath::RandHelper(5);
	if (RamdonNum == 1 && AmmoPickupClass)
	{
		RequestController->GetWorld()->SpawnActor<ABFPickup_Ammo>(AmmoPickupClass,RequestController->GetPossessedZombie()->GetActorLocation(), RequestController->GetControlRotation(), PickupsSpawnParameters);
	}
	if (RamdonNum == 3 && HealthPickupClass)
	{
		RequestController->GetWorld()->SpawnActor<ABFPickup_Health>(HealthPickupClass,RequestController->GetPossessedZombie()->GetActorLocation(), RequestController->GetControlRotation(), PickupsSpawnParameters);
	}
}

void ABFGameMode_Evasion::OnKillZombie(ABFPlayerController* Killer)
{
	ReduceZombie(1);
}

void ABFGameMode_Evasion::ReduceZombie(int32 ReduceZombieNum)
{
	SurviveTheSurgeZombieLeft -= ReduceZombieNum;
}


