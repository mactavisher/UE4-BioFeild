// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFInventoryItem.generated.h"

class AActor;
class ABFBaseCharacter;
class ABFPlayerCharacter;
class ABFPlayerController;

/**
 * base class for inventory items such as weapons and things that can be added to inventory
 */
UCLASS()
class BIOFEILD_API ABFInventoryItem : public AActor
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:
	/** receive detected event if detected */
	virtual void ReceiveDetected(class AActor* DetectedBy, class  ABFBaseCharacter* DectectedCharacter, class ABFPlayerController* DectedPlayer);
};
