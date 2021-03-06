// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BioFeild.h"
#include "BFPlayerController.generated.h"

class ABFPlayerCharacter;
class UArrowComponent;
class ABFInventoryItem;
/**
*
*/
UCLASS(notplaceable)
class BIOFEILD_API ABFPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controll|TimeDialation", meta = (ClampMin = "0.01", ClampMax = "10"))
		float GlobalTimeDialationAmount;

#if WITH_EDITORONLY_DATA
	/**  Arrow comp help to point the direction of this Controller,if with editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ArrowComp", meta = (AllowPrivateAccess = "true"))
		UArrowComponent*  ArrowComp;
#endif

protected:
	/**cached specified possessed Character  */
	UPROPERTY()
	ABFPlayerCharacter* PoccessedPlayerCharacter;

	UPROPERTY()
	int32  Score;

	UPROPERTY()
	ETeam InTeam;

protected:
	virtual void BeginPlay()override;

public:
	virtual void Possess(APawn* Pawn)override;

	virtual void UnPossess()override;

	virtual void AddScore(int32 Score);

	virtual void EnableSlowMotion();

	virtual void DisableSlowMotion();

	virtual void SetTeam(ETeam InTeam) { this->InTeam = InTeam; }

	virtual  ETeam GetTeam()const { return InTeam; }

	virtual void ShowItemInfo(ABFInventoryItem* ItemToshow);

	/** return current controlled character */
	virtual ABFPlayerCharacter* GetPoccessedPlayerCharacter()const { return PoccessedPlayerCharacter; }

	virtual void SetPoccessedPlayerCharacter(ABFPlayerCharacter* PlayerCharacter) { PoccessedPlayerCharacter = PlayerCharacter; }
};
