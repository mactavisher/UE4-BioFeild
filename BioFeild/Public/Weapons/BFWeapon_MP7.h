// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BFWeapon_SubMachine.h"
#include "BFWeapon_MP7.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFWeapon_MP7 : public ABFWeapon_SubMachine
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
