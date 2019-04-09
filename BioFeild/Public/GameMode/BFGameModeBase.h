// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BFGameModeBase.generated.h"


class ABFPlayerController;
/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFGameModeBase : public AGameMode
{
	GENERATED_UCLASS_BODY()

public:
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rules")
		float TeamDamageModifier;

		virtual void ModifyDamage(float& Outdamage, AController* DamageCauser, AController*DamagedPlayer);
	
};
