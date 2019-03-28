// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "GameMode_Menu.generated.h"

class ABFPlayerCharacter;
class ABFPlayerController_Menu;
class UGameplayStatics;
class UObjectLibrary;

/**
 *  Menu game mode class 
 */
UCLASS()
class BIOFEILD_API AGameMode_Menu : public AGameMode
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	ABFPlayerCharacter* Character;

	UPROPERTY()
	ABFPlayerController_Menu* MenuPlayerController;

	FStreamableManager StreamableManager;

	// Asset library for all levels
	UPROPERTY()
		UObjectLibrary * LevelLibrary;

	// Asset library for weapon classes
	UPROPERTY()
		UObjectLibrary * WeaponLibrary;

	// Asset library for scope classes
	UPROPERTY()
		UObjectLibrary * ScopeLibrary;

	// Asset library for Silencer classes
	UPROPERTY()
		UObjectLibrary * SilencerLibaray;

	UPROPERTY()
		UObjectLibrary * ProjectileLibrary;

	virtual void BeginPlay()override;

	virtual void GetWeaponClasses(UObjectLibrary* ObjectLibrary, TArray<TSubclassOf<ABFWeaponBase>> &OutWeaponClasses);

	virtual void GetWeaponScopeClasses(UObjectLibrary* ObjectLibary, TArray<TSubclassOf<ABFAttachment_Scope>>& OutWeaponScopeClassess);
    
	virtual void GetLevels(TArray<FAssetData>& LevelAsset);
};
