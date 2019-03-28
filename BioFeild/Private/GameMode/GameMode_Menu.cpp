// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_Menu.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/Engine.h"
#include "Projectile/BFProjectile.h"
#include "Attachments/BFAttachment_Scope.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Character/BFPlayerController_Menu.h"

AGameMode_Menu::AGameMode_Menu(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PlayerControllerClass = ABFPlayerController_Menu::StaticClass();
	/** initialize level library */
	LevelLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, GIsEditor);
	LevelLibrary->AddToRoot();
	LevelLibrary->LoadAssetDataFromPath(TEXT("/Game/Maps"));

	/** initialize weapon library */
	WeaponLibrary = UObjectLibrary::CreateLibrary(ABFWeaponBase::StaticClass(), true, GIsEditor);
	WeaponLibrary->AddToRoot();
	WeaponLibrary->LoadBlueprintsFromPath(FString("/Game/Blueprints/Weapons"));

	/** Initializing weapon scope classes */
	ScopeLibrary = UObjectLibrary::CreateLibrary(ABFAttachment_Scope::StaticClass(), true, GIsEditor);
	ScopeLibrary->AddToRoot();
	ScopeLibrary->LoadBlueprintsFromPath(TEXT("Game/Blueprints/Weapons/Attachments/Scope"));
}



void AGameMode_Menu::BeginPlay()
{
	Super::BeginPlay();
}

void AGameMode_Menu::GetWeaponClasses(UObjectLibrary* ObjectLibrary, TArray<TSubclassOf<ABFWeaponBase>> &OutWeaponClasses)
{
	TArray<class UBlueprintGeneratedClass*> WeaponAssetList;
	WeaponLibrary->GetObjects<UBlueprintGeneratedClass>(WeaponAssetList);
	for (int32 i = 0; i <= WeaponAssetList.Num() - 1; i++)
	{
		UBlueprintGeneratedClass* WeaponBlueprintClass = WeaponAssetList[i];
		if (!WeaponBlueprintClass->GetName().StartsWith("SKEL_"))
		{
			OutWeaponClasses.AddUnique(WeaponBlueprintClass);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, WeaponBlueprintClass->GetName());
		}
	}
}

void AGameMode_Menu::GetWeaponScopeClasses(UObjectLibrary* ObjectLibary, TArray<TSubclassOf<ABFAttachment_Scope>>& OutWeaponScopeClassess)
{
	TArray<class UBlueprintGeneratedClass*> WeaponScopeBlueprintClasses;
	ScopeLibrary->GetObjects<UBlueprintGeneratedClass>(WeaponScopeBlueprintClasses);
	for (int32 i = 0; i <= WeaponScopeBlueprintClasses.Num() - 1; i++)
	{
		UBlueprintGeneratedClass* WeaponBlueprintClass = WeaponScopeBlueprintClasses[i];
		if (!WeaponBlueprintClass->GetName().StartsWith("SKEL_"))
		{
			OutWeaponScopeClassess.AddUnique(WeaponBlueprintClass);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, WeaponBlueprintClass->GetName());
		}
	}
}

void AGameMode_Menu::GetLevels(TArray<FAssetData>& LevelAsset)
{
	LevelLibrary->GetAssetDataList(LevelAsset);
}

