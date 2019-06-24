// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFInventoryItem.generated.h"

class AActor;
class ABFBaseCharacter;
class ABFPlayerCharacter;
class ABFPlayerController;
class UTexture2D;

/**
 * base class for inventory items such as weapons and things that can be added to inventory
 */
UCLASS()
class BIOFEILD_API ABFInventoryItem : public AActor
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D*  ItemTexture2d;

	/** indicates this item is a kind of collectible item */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Props")
		uint8 bIsCollectibleItem : 1;

	/** if this is a collect able item , indicates if this item can be collected right now */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Props")
		uint8 bCanCollect : 1;

protected:
	virtual void BeginPlay() override;

public:
	/** receive detected event if detected */
	virtual void ReceiveDetected(class AActor* DetectedBy, class  ABFBaseCharacter* DectectedCharacter, class ABFPlayerController* DectedPlayer);

	FORCEINLINE UTexture2D* GetItemTexture2D()const { return ItemTexture2d; }
};
