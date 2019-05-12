// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BFWeapon_FullAutomatic.h"
#include "BFWeapon_Scar.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFWeapon_Scar : public ABFWeapon_FullAutomatic
{
	GENERATED_UCLASS_BODY()
	
protected:

	virtual void BeginPlay()override;

	virtual void PostInitializeComponents()override;

	virtual FVector GetADSCameraAdjustVector()const override;
};
