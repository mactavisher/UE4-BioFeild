// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BFUserWidgetBase.h"
#include "BFCurrentWeaponWidget.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API UBFCurrentWeaponWidget : public UBFUserWidgetBase
{
	GENERATED_UCLASS_BODY()


	UPROPERTY()
		ESlateVisibility CrossHairVisiblity;

	UPROPERTY()
		ESlateVisibility  ConfirmHitVisibility;

	UPROPERTY()
		ESlateVisibility  ReloadVisibility;

	UPROPERTY()
		FLinearColor CrossHairColor;

	UPROPERTY()
		FLinearColor TargetHitColor;

	UPROPERTY()
		class  ABFWeaponBase* CurrentWeapon;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		virtual ESlateVisibility GetCrossHairVisibility()const;

	UFUNCTION(BlueprintCallable,Category="UI")
		virtual void SetCrossHairVisibility(ESlateVisibility NewVisibility) { this->CrossHairVisiblity = NewVisibility; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		virtual ESlateVisibility GetConfirmHitVisibility()const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		virtual void SetConfirmHitVisibility(ESlateVisibility NewVisibility);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		virtual ESlateVisibility GetReloadVisibility()const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		virtual FLinearColor GetCrossHairColor()const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		virtual FLinearColor GetHitTargetColor()const;

	UFUNCTION(BlueprintSetter,Category = "UI")
		virtual void SetCurrentWeapon(ABFWeaponBase* NewWeapon) { CurrentWeapon = NewWeapon; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		virtual ABFWeaponBase* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "UI")
		virtual void SetReloadVisibility(ESlateVisibility NewVisibility) { this->ReloadVisibility = NewVisibility; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		virtual int32 GetCurrentAmmo()const;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "UI")
		virtual int32 GetLeftAmmo()const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
		virtual UTexture2D* GetWeaponIConTexture()const;
	
};
