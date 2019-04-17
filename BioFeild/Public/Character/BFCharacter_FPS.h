// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BFPlayerCharacter.h"
#include "BFCharacter_FPS.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFCharacter_FPS : public ABFPlayerCharacter
{
	GENERATED_UCLASS_BODY()

protected:

	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTimeSeconds)override;
};
