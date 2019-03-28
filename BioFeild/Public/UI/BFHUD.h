// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/BFUserWidgetBase.h"
#include "BFHUD.generated.h"


class ABFPlayerController;
class UBFUserWidgetBase;
class ABFWeaponBase;
/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFHUD : public AHUD
{
	GENERATED_UCLASS_BODY()
	
private:
	/** owner controller of this HUD*/
	UPROPERTY()
	ABFPlayerController* OwnerController;

	UPROPERTY()
	UBFUserWidgetBase* CrossHairWidget;

	UPROPERTY()
	UBFUserWidgetBase* WeaponInfoWidet;

	UPROPERTY()
	UBFUserWidgetBase* TargetHitWidget;

	UPROPERTY()
	uint8 bShouldHideHUD : 1;

	UPROPERTY()
	uint8 bShouleHideCrossHair : 1;

	UPROPERTY()
	ABFWeaponBase* CurrentPlayerWeapon;


public:
	/** HUD Draw Loop */
	virtual void DrawHUD()override;

	/** draw weapon cross hair */
	virtual void DrawCorsshair();

	virtual void BeginPlay()override;

	/** initialize all widget for this HUD */
	virtual void InitializeWidgets();

	/** Hide HUD and all it's elements */
	virtual void TurnOffHUD();

	/** hide weapon cross hair */
	virtual void TurnoffCrossHair();

};
