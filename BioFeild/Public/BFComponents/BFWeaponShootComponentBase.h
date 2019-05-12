// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/BFWeaponBase.h"
#include "Engine/World.h"
#include "BFWeaponShootComponentBase.generated.h"

class ABFWeaponBase;

/**
 * this component gives weapon the ability to fire in deferent ways
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOFEILD_API UBFWeaponShootComponentBase : public UActorComponent
{
	GENERATED_UCLASS_BODY()

protected:

	/** weapon that own this component */
	UPROPERTY(BlueprintReadOnly)
		ABFWeaponBase* WeaponOwner;

	/** indicate whether this component need to decrease weapon Spread,used in runtime */
	UPROPERTY(BlueprintReadOnly)
		uint8 bNeedRestoreWeaponSpread : 1;

	/** indicate whether this component need to increase weapon Spread,used in runtime*/
	UPROPERTY(BlueprintReadOnly)
		uint8 bNeedIncreaseWeaponSpread : 1;

	/** indicate whether this component should cause weapon recoil ,used in runtime*/
	UPROPERTY(BlueprintReadOnly)
		uint8 bUseRecoil : 1;

	UPROPERTY(BlueprintReadOnly)
		float CurrentRecoilValue;

	/** record last shoot time used for calculate shooting intervals ,used in runtime*/
	UPROPERTY()
		float LastShootTime;

public:
	/** base recoil value when start recoil */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Recoil")
		float BaseRecoilValue;

protected:
	/** called in every frame, when in game play , should be turned off to improve game performance when this component currently not needed*/
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** base behavior weapon will do in each shoot  */
	virtual void EachSingleShoot();

	/** increase weapon Spread logic here */
	virtual void IncreaseWeaponSpread(float Value);

	/** decrease weapon Spread logic here */
	virtual void DecreaseWeaponSpread(float Value);

	/** called on game starts or this spawned */
	virtual void BeginPlay() override;

public:

	/** enable this to tick */
	UFUNCTION(BlueprintCallable,Category="ShootComponent")
	virtual void EnableComponentTick();

	/** disable this ticking */
	UFUNCTION(BlueprintCallable, Category = "ShootComponent")
	virtual void DisableComponentTick();

	/** set base recoil value */
	UFUNCTION(BlueprintCallable,BlueprintSetter,Category="Recoil")
	virtual void SetBaseRecoilValue(float Value) { this->BaseRecoilValue = Value; };

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Recoil")
	virtual float GetBaseRecoilValue()const { return BaseRecoilValue; };

	/** set base recoil value */
	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "Recoil")
		virtual void SetCurrentRecoilValue(float Value) { this->CurrentRecoilValue = Value; };

	/** get current recoil value */
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Recoil")
	virtual float GetCurrentRecoilValue()const { return CurrentRecoilValue; }

	/** Owner setter*/
	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "OwnerWeapon")
	virtual void SetWeaponOwner(ABFWeaponBase* NewOwner) { this->WeaponOwner = NewOwner; }

	/** owner getter */
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "OwnerWeapon")
	virtual ABFWeaponBase* GetWeaponOwner()const { return WeaponOwner; }

};
