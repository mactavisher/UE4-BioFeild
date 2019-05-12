// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BFWeaponBase.h"
#include "BFWeapon_HandGun.generated.h"

class UBFWeaponSingleShootComponent;
/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFWeapon_HandGun : public ABFWeaponBase
{
	GENERATED_UCLASS_BODY()

		/** Weapon single shoot component */
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeponMesh", meta = (AllowPrivateAccess = "true"))
		UBFWeaponSingleShootComponent* SingleShotComp;
	
protected:
	virtual void Tick(float DeltaTime)override;

	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;

	virtual void Fire()override;

	virtual void StopFire()override;

	/** sub machine gun only have single shoot component, so we can't switch mode */
	virtual void ToggleFireMode()override;

	virtual FVector GetADSCameraAdjustVector()const override;

	virtual void SetupAttachments()override;

	virtual void OnWeaponEquipingFinished()override;

	virtual void OnWeaponUnequipingFinished()override;
};
