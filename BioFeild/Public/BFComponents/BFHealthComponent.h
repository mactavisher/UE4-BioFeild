// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BFHealthComponent.generated.h"

class ABFBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShouldDieSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReduceHealthSignature,float,ReduceAmount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOFEILD_API UBFHealthComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		float DefaultHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		float MaximunHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		float LowHealthPercentage;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnShouldDieSignature OnCharacterShouldDie;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnLowHealthSignature OnLowHealth;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnReduceHealthSignature OnHealthReduced;

protected:

	/** current health value,changes in run time */
	UPROPERTY()
	float CurrentHealth;

	/** character that own this component */
	ABFBaseCharacter* OwnerCharacter;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
		virtual  float GetDefaultHealth()const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
		virtual  float GetCurrentHealth()const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void SetHealth(float Health);

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ReduceHealth(float ReduceAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ReGenerateHealth(float GenerateAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual float GetLowHealthPercentage()const { return LowHealthPercentage; }

	/** owner character setter */
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void SetOwnerCharacter(ABFBaseCharacter* NewOwnerCharacter) { OwnerCharacter = NewOwnerCharacter; }

	/** owner character getter */
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual ABFBaseCharacter* GetOwnerCharacter()const { return OwnerCharacter; }

	UFUNCTION(BlueprintCallable,Category="Health")
		virtual float GetHealthPercentage();

	    virtual bool CheckIsLowHealth();

	    virtual void BeginPlay()override;

		virtual void EnableComponentTick();

		virtual void DisableComponentTick();

		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

};
