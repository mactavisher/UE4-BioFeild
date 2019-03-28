// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BFPlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "BFPlayerController_Menu.generated.h"

class ABFPlayerCharacter;
class UGameplayStatics;
class ABFProjectile;

/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFPlayerController_Menu : public APlayerController
{
	GENERATED_UCLASS_BODY()

	UPROPERTY()
	ABFPlayerCharacter* LobbyCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LobbyMusic")
		UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "LobbyMusic")
		USoundBase* LobbyMusic;



public:
	virtual void Possess(APawn* Pawn)override;

	virtual void UnPossess()override;
  
	virtual void BeginPlay()override;

	virtual ABFPlayerCharacter* GetPlayerCharacter()const { return LobbyCharacter; }
	
};
