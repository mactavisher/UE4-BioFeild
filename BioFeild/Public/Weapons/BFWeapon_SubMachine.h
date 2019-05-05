// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BFWeaponBase.h"
#include "BFWeapon_SubMachine.generated.h"

class UBFWeaponBurstShootComponent;
/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFWeapon_SubMachine : public ABFWeaponBase
{
	GENERATED_UCLASS_BODY()
	
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BurstShootComp", meta = (AllowPrivateAccess = "true"))
		UBFWeaponBurstShootComponent* BurstShootComp;
	
protected:
	virtual void Tick(float DeltaTime)override;

	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;

	virtual void Fire()override;

	virtual void StopFire()override;

	/** sub machine gun only have single shoot component, so we can't switch mode */
	virtual void ToggleFireMode()override;

	virtual void OnWeaponEquiped()override;

	virtual void OnWeaponUnEquiped()override;
	
};
