// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BFComponents/BFWeaponShootComponentBase.h"
#include "BFWeaponSingleShootComponent.generated.h"

/**
 * 
 */
class ABFWeaponBase;
UCLASS()
class BIOFEILD_API UBFWeaponSingleShootComponent : public UBFWeaponShootComponentBase
{
	GENERATED_UCLASS_BODY()
	
private:
	FTimerHandle SingleShotCoolDownHandle;

	virtual void ShootCoolDown();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;

protected:
	virtual void BeginPlay()override;

public:
	virtual void SimulateSingleShoot();
};
