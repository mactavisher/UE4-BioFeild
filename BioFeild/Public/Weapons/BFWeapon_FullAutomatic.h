// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BFWeaponBase.h"
#include "BFWeapon_FullAutomatic.generated.h"

class UBFWeaponSingleShootComponent;

class UBFWeaponTripleShootComponent;

class UBFWeaponBurstShootComponent;
/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFWeapon_FullAutomatic : public ABFWeaponBase
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SingleShotComp", meta = (AllowPrivateAccess = "true"))
		UBFWeaponSingleShootComponent* SingleShootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BurstShootComp", meta = (AllowPrivateAccess = "true"))
		UBFWeaponBurstShootComponent* BurstShootComp;


protected:
	virtual void Tick(float DeltaTime)override;


	virtual void PostInitializeComponents()override;


	virtual void BeginPlay()override;
	
	virtual void Fire()override;

	virtual void StopFire()override;

	virtual void ToggleFireMode()override;
};
