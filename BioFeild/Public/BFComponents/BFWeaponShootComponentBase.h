// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/BFWeaponBase.h"
#include "Engine/World.h"
#include "BFWeaponShootComponentBase.generated.h"

class ABFWeaponBase;

/**
 * this component gives weapon the ability to fire
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOFEILD_API UBFWeaponShootComponentBase : public UActorComponent
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** weapon that own this component */
	UPROPERTY()
		ABFWeaponBase* WeaponOwner;

	UPROPERTY()
		uint8 bNeedRestoreWeaponSpread : 1;

	UPROPERTY()
		uint8 bNeedIncreaseWeaponSpread : 1;

	UPROPERTY()
		uint8 bUseRecoil:1;

	/** record last shoot time used for calculate shooting intervals */
	UPROPERTY()
		float LastShootTime;

	UPROPERTY()
		float BaseRecoilValue;

	UPROPERTY()
		float CurrentRecoilValue;

public:
	/** Owner setter*/
	virtual void SetWeaponOwner(ABFWeaponBase* NewOwner) { this->WeaponOwner = NewOwner; }

	/** owner getter */
	virtual ABFWeaponBase* GetWeaponOwner()const { return WeaponOwner; }

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** base behavior weapon will do in each shoot  */
	virtual void EachSingleShoot();

	virtual void IncreaseWeaponSpread(float Value);

	virtual void DecreaseWeaponSpread(float Value);

public:
	virtual void EnableComponentTick();

	virtual void DisableComponentTick();
};
