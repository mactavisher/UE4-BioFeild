// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "BFProjectileDamage.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API UBFProjectileDamage : public UDamageType
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Message")
		FString KillMessage;
};
