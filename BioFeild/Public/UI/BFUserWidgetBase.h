// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BFUserWidgetBase.generated.h"

class ABFPlayerController;
/**
 * 
 */
UCLASS()
class BIOFEILD_API UBFUserWidgetBase : public UUserWidget
{
	GENERATED_UCLASS_BODY()

public:
	virtual void ShowWidget();

	virtual void HideWidget();
};
