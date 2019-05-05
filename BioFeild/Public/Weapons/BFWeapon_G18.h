// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BFWeapon_HandGun.h"
#include "BFWeapon_G18.generated.h"

/**
 *  G_18 hand gun 
 */
UCLASS()
class BIOFEILD_API ABFWeapon_G18 : public ABFWeapon_HandGun
{
	GENERATED_UCLASS_BODY()

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
};
