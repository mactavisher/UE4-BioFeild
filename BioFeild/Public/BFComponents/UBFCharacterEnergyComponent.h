// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UBFCharacterEnergyComponent.generated.h"


class ABFBaseCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObCharacterExhaustedSignature);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BIOFEILD_API UUBFCharacterEnergyComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	/** character base energy value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnergyCompProperty")
		float EnergyValueBase;

	/** current character energy value,blueprint read only*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnergyCompProperty")
		float CurrentEnergyValue;

	/** character max energy value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnergyCompProperty")
		float MaxEnergyValue;

	/** character energy decrement base */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnergyCompProperty")
		float EnergyDecrement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnergyCompProperty")
		float EnergyIncrement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EnergyCompProperty")
		ABFBaseCharacter* CharacterOwner;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FObCharacterExhaustedSignature OnCharacterExhausted;

	/** indicates whether character should restore energy */
	UPROPERTY()
		uint8 bShouldRestroeEnergy : 1;

	/** indicates whether character should decrease energy */
	UPROPERTY()
		uint8 bShouldDecreaseEnergy : 1;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @desc     upgrade every tick event
	 * @param    DeltaTime            tick interval between frame                float
	 * @param    TickType             tickType                                   ELevleTick
	 * @param    ThisTickFunction     TickFunction                               FActorComponentTickFunction
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;

	/**
	 * @desc   character energy start decrease logic here
	 * @param  DecreaseValue            how much to decrease energy value    float
	 */
	virtual void OnEnergyStartDecrease(float DecreaseValue);

	/**
	 * @desc   character energy start increase logic here
	 * @param  IncreamentValue            how much to increase energy value    float
	 */
	virtual void OnEnergyStartRestore(float IncreamentValue);

public:

	/**
	 * @desc character owner setter
	 * @params OwnerCharacter                     owner character of this component             ABFBaseCharacter
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
		virtual void SetCharacterOwner(ABFBaseCharacter* OwnerCharacter) { this->CharacterOwner = OwnerCharacter; }

	/**
	 * @desc          owner character getter
	 * @return        owner character of this component             ABFBaseCharacter
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
		virtual ABFBaseCharacter* GetCharacterOwner()const { return CharacterOwner; }

	/**
	* @desc          getter of character current energy value
	* @return        character current energy value            float
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
		virtual float GetCurrentEnergyValue()const { return CurrentEnergyValue; }

	/**
	 * @desc      setter of character current energy
	 * @param     value of energy to set for this component          float
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
		virtual void SetCurrentEnergyValue(float Value) { this->CurrentEnergyValue = Value; }
};
