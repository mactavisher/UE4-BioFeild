// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BioFeild.h"
#include "GameMode/BFGameModeBase.h"
#include "BFGameMode_Evasion.generated.h"

class ABFPlayerController;
class ABFZombieController;
/** game difficulty types */
UENUM(BlueprintType)
enum class EGameDifficulty :uint8 {
	easy,
	medium,
	hard,
	insane,
};

/** we got 3 stages for this game mode */
namespace EGameLevel
{
	enum Type
	{
		SurviveTheSurge,
		TurnnelRush,
		Evasion,
	};
}

/**
*  Game mode for zombie evasion
*/

UCLASS()
class BIOFEILD_API ABFGameMode_Evasion : public ABFGameModeBase
{

	GENERATED_UCLASS_BODY()

public:

	/** ordinal zombie types */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ZombieType")
		TArray<TSubclassOf<class ABFZombie>> ZombieTypeClasses;

	/** ammo pack reward for player killing zombies */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ZombieType")
		TSubclassOf<class ABFPickup_Ammo> AmmoPickupClass;

	/** health reward pack for player killing zombies */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ZombieType")
		TSubclassOf<class ABFPickup_Health> HealthPickupClass;

	/** player maximum lives to re_spawn after dead  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ZombieType")
		uint8 MaximunLives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameConfig")
		EGameDifficulty GameDifficulty;

private:
	/** enable team damage? turned off by default */
	UPROPERTY(Transient)
		uint8 bAllowTeamDamage : 1;

	UPROPERTY()
		uint8 CurrentLivesLeft;

	/** current game level player is in  */
	EGameLevel::Type  CurrentGameLevel;

	UPROPERTY()
		int32 SurviveTheSurgeZombies;

	UPROPERTY()
		int32 SurviveTheSurgeZombieLeft;

	UPROPERTY()
		int32 ZombieSpawned;

	UPROPERTY()
		float TimeBetweenWave;

	UPROPERTY()
		float PrepareTime;

	UPROPERTY()
		FTimerHandle WaveStartTimerHandle;

	UPROPERTY()
	    FTimerHandle GameStartTimerHandle;

public:
	virtual void ResetCurrentLivesLeft();

	virtual void BeginPlay()override;

	virtual void StartMatch()override;

	virtual void EndMatch()override;

	virtual void SpawnPickups(ABFZombieController*  RequestController);

	virtual int32 GetSurveTheSurgeZombieLeft()const { return SurviveTheSurgeZombies; };

	virtual int32 GetSurveTheSurgeZombies()const { return SurviveTheSurgeZombieLeft; };

	virtual void OnKillZombie(ABFPlayerController* Killer);

	virtual void ReduceZombie(int32 ReduceZombieNum);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)override;

	virtual void Prepare();

	virtual void FinishPrePare();

	virtual void FinishCurrentWave();

	virtual void ModifyDamage(float& Outdamage, AController* DamageCauser, AController*DamagedPlayer,FDamageEvent DamageEvent)override;
};
