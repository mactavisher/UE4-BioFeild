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
	UPROPERTY()
	ABFBaseCharacter* OwnerCharacter;

	virtual void BeginPlay()override;

public:
	virtual void SetSprintSpeed();

	virtual void SetAimingSpeed();

	virtual void SetCrouchSpeed();

	virtual void SetADSSpeed();

	virtual void SetDefaultMaxWalkSpeed();

	virtual float GetDefaultMaxWalkSpeed()const { return DefaultMaxWalkSpeed; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual ABFBaseCharacter* GetOwnerCharacter()const { return OwnerCharacter; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual void SetOwnerCharacter(ABFBaseCharacter* Owner);
};
