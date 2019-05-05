// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BFWeapon_FullAutomatic.h"
#include "BFWeapon_AK47.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFWeapon_AK47 : public ABFWeapon_FullAutomatic
{
	GENERATED_UCLASS_BODY()

protected:

	virtual void Tick(float DeltaTime)override;

	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;

	virtual void Fire()override;

	virtual void StopFire()override;

	virtual void ToggleFireMode()override;

	virtual FVector GetADSCameraAdjustVector()const override;
};
