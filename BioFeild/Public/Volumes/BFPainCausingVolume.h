// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PainCausingVolume.h"
#include "BFPainCausingVolume.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFPainCausingVolume : public APainCausingVolume
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="PainVolume")
		uint8 bCausingPainToPlayerCharacter : 1;

		virtual void CausePainTo(class AActor* Other)override;
};
