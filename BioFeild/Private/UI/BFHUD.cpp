// Fill out your copyright notice in the Description page of Project Settings.

#include "BFHUD.h"
#include "Character/BFPlayerController.h"
#include "Character/BFPlayerCharacter.h"
#include"Weapons/BFWeaponBase.h"

ABFHUD::ABFHUD(const FObjectInitializer&Objectinitializer) :Super(Objectinitializer)
{
	bShouldHideHUD = false;
	bShouleHideCrossHair = false;
}

void ABFHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!bShouldHideHUD)
	{
		if (!bShouleHideCrossHair)
		{
			//DrawCorsshair();
		}
	}
}

void ABFHUD::DrawCorsshair()
{
	if (CrossHairWidget)
	{
		CrossHairWidget->AddToViewport(0);
	}
}

void ABFHUD::BeginPlay()
{
	Super::BeginPlay();
	//OwnerController = Cast<ABFPlayerController>(PlayerOwner);
	//CurrentPlayerWeapon = OwnerController->GetPoccessedPlayerCharacter()->GetCurrentWeapon();
	//InitializeWidgets();
}

void ABFHUD::InitializeWidgets()
{
	if (OwnerController)
	{
		if (CurrentPlayerWeapon)
		{
			CrossHairWidget = CurrentPlayerWeapon->GetCrossHairWidgetInstance();
			TargetHitWidget = CurrentPlayerWeapon->GetTargetHitFeedBackWidgetInstance();
			WeaponInfoWidet = CurrentPlayerWeapon->GetWeaponInfoWidgetInstace();
		}
	}
}

void ABFHUD::TurnOffHUD()
{
	bShouldHideHUD = true;
}

void ABFHUD::TurnoffCrossHair()
{
	bShouleHideCrossHair = true;
}
