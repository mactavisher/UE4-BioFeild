// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BFHealthComponent.generated.h"

class ABFBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShouldDieSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLowHealthSignature);

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

protected:
	float CurrentHealth;

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

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void SetOwnerCharacter(ABFBaseCharacter* NewOwnerCharacter) { OwnerCharacter = NewOwnerCharacter; }

	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual ABFBaseCharacter* GetOwnerCharacter()const { return OwnerCharacter; }

	UFUNCTION()
		virtual float GetHealthPercentage();

	virtual bool CheckIsLowHealth();

	virtual void BeginPlay()override;
};
