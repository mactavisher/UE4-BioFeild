// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BFComponents/BFWeaponShootComponentBase.h"
#include "BFWeaponBurstShootComponent.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API UBFWeaponBurstShootComponent : public UBFWeaponShootComponentBase
{
	GENERATED_UCLASS_BODY()
	
protected:

	UPROPERTY()
	FTimerHandle BurstShootTimerHandle;

public:
	virtual void SimulateBurstShoot();
	virtual void FinishsBurstShoot();
	virtual void EnableComponentTick()override;
	virtual void DisableComponentTick()override;
protected:
	virtual void BeginPlay()override;
	virtual void EachSingleShoot() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;
};
