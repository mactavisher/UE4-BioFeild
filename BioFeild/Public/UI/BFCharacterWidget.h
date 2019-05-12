// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BFUserWidgetBase.h"
#include "BFCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API UBFCharacterWidget : public UBFUserWidgetBase
{
	GENERATED_UCLASS_BODY()

        UFUNCTION(BlueprintCallable,BlueprintPure,Category = "CharacterWidget")
		virtual float GetHealthPercentage()const;
	
};
