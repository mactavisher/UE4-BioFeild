// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BFCharacterMovementComponent.generated.h"

class ABFBaseCharacter;
/**
 * 
 */

UCLASS()
class BIOFEILD_API UBFCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MoveMents", meta = (ClampMin = "0", ClampMax = "3"))
		float SprintSpeedModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MoveMents", meta = (ClampMin = "0", ClampMax = "1"))
		float AimingSpeedModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MoveMents", meta = (ClampMin = "0", ClampMax = "1"))
		float CrouchSpeedModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MoveMents", meta = (ClampMin = "0", ClampMax = "1"))
		float ADSSpeedModifier;

	UPROPERTY()
		float DefaultMaxWalkSpeed;



protected:

	UPROPERTY(Transient)
	ABFBaseCharacter* OwnerCharacter;

	virtual void BeginPlay()override;

public:
	virtual void SetSprint();

	virtual void SetAiming();

	virtual void SetCrouch();

	virtual void SetADS();

	virtual void SetDefault();

	virtual float GetDefaultMaxWalkSpeed()const { return DefaultMaxWalkSpeed; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual ABFBaseCharacter* GetOwnerCharacter()const { return OwnerCharacter; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual void SetOwnerCharacter(ABFBaseCharacter* Owner);
};
