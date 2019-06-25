// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BFHealthComponent.generated.h"

class ABFBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShouldDieSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReduceHealthSignature, float, ReduceAmount);

/**
 * character health component, each pawn of this game should have this component 
 * to represent their health state
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BIOFEILD_API UBFHealthComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	/** character's default health value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
		float DefaultHealth;
	/** character's maximum health value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
		float MaximunHealth;

	/**low health percentage hit value when character is not in full health  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
		float LowHealthPercentage;

	/** event syntax when character should die */
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnShouldDieSignature OnCharacterShouldDie;

	/** event syntax when character is in low health state */
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnLowHealthSignature OnLowHealth;

	/** event syntax when character health value reduced */
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnReduceHealthSignature OnHealthReduced;

protected:
	/** current health value,changes in run time */
	UPROPERTY(BlueprintReadOnly, Category = "HealthComponent")
		float CurrentHealth;

	/** character that own this component */
	UPROPERTY(BlueprintReadOnly, Category = "HealthComponent")
		ABFBaseCharacter* OwnerCharacter;

	/**
	 * @override  component begin play event
	 */
	virtual void BeginPlay()override;

	/**
     * @override component tick logic here
     * @params     DeltaTime              frame tick interval time              float
     * @params     TickType               level tick type                       ELevelTick
     * @params     ThisTickFunction       tick function                         FActorComponentTickFunction*
     */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

public:
	/**
	 * @desc getter for default health value
	 * @return  default health value              float
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HealthComponent")
		virtual  float GetDefaultHealth()const { return CurrentHealth; }

	/**
	 * @desc getter for current health value
	 * @return current health value              float
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HealthComponent")
		virtual  float GetCurrentHealth()const { return CurrentHealth; }

	/**
	 * @desc setter for current health value
	 * @param Health                             float
	 */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		virtual void SetHealth(float Health);

	/**
	 * @desc health reduce logic here
	 * @param ReduceAmount          how much health to reduce        float
	 */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		virtual void ReduceHealth(float ReduceAmount);

	/**
	 * @desc restore current health value
	 * @param  GenerateAmount       how much health to restore        float
	 */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		virtual void ReGenerateHealth(float GenerateAmount);

	/**
	 * @desc getter for low low health percentage
	 * @return  LowHealthPercentage     LowHealthPercentage        float
	 */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		virtual float GetLowHealthPercentage()const { return LowHealthPercentage; }

	/**
	 * @desc setter the character that own this component
	 * @params    NewOwnerCharacter            new owner character to set            ABFBaseCharacter
	 */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		virtual void SetOwnerCharacter(ABFBaseCharacter* NewOwnerCharacter) { OwnerCharacter = NewOwnerCharacter; }

	/**
	 * @desc getter for OwnerCharacter
	 * @return  OwnerCharacter     owner character of this component        ABFBaseCharacter
	 */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		virtual ABFBaseCharacter* GetOwnerCharacter()const { return OwnerCharacter; }

	/**
	 * @desc getter for current health percentage
	 * @return  health percentage     owner character current health percentage        float
	 */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		virtual float GetHealthPercentage();

	/**
	 * @desc check to see if current health hit low health percentage
	 * @return true or false           is low health?                     bool
	 */
	virtual bool CheckIsLowHealth();

	/**
	 * enable this component tick logic
	 */
	virtual void EnableComponentTick();

	/**
	 * disable this component tick logic
	 */
	virtual void DisableComponentTick();
};
