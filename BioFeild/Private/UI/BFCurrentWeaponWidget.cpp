// Fill out your copyright notice in the Description page of Project Settings.

#include "BFCurrentWeaponWidget.h"
#include "Character/BFPlayerCharacter.h"
#include "Weapons/BFWeaponBase.h"

UBFCurrentWeaponWidget::UBFCurrentWeaponWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

	ReloadVisibility = ESlateVisibility::Collapsed;
	CrossHairVisiblity = ESlateVisibility::Visible;
	ConfirmHitVisibility = ESlateVisibility::Hidden;
}

ESlateVisibility UBFCurrentWeaponWidget::GetCrossHairVisibility() const
{
	ABFPlayerController* OwnerCurrentPlayer = Cast<ABFPlayerController>(GetOwningPlayer());
	if (OwnerCurrentPlayer)
	{
		ABFPlayerCharacter* OwningCurrentCharacter = OwnerCurrentPlayer->GetPoccessedPlayerCharacter();
		if (OwningCurrentCharacter)
		{
			ABFWeaponBase* PlayerCurrentWeapon = OwningCurrentCharacter->GetCurrentWeapon();
			if (PlayerCurrentWeapon&&!OwningCurrentCharacter->GetCharacterIsADS()&&PlayerCurrentWeapon->GetWeaponState()!=EWeaponState::Reload)
			{
				return ESlateVisibility::Visible;
			}
			return ESlateVisibility::Collapsed;
		}
		return ESlateVisibility::Collapsed;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UBFCurrentWeaponWidget::GetConfirmHitVisibility() const
{
	ABFPlayerController* OwnerCurrentPlayer = Cast<ABFPlayerController>(GetOwningPlayer());
	if (OwnerCurrentPlayer)
	{
		ABFPlayerCharacter* OwningCurrentCharacter = OwnerCurrentPlayer->GetPoccessedPlayerCharacter();
		if (OwningCurrentCharacter)
		{
			const FTargetHitInfo TargetHitInfo = OwningCurrentCharacter->GetTargetHitInfo();
			//UE_LOG(LogTemp, Warning, TEXT("Hit target in-->victim %s"), TargetHitInfo.bIsTargetDead)
			if (TargetHitInfo.DamgeCause>0)
			{
				return ESlateVisibility::Visible;
			}
			return ESlateVisibility::Collapsed;
		}
		return ESlateVisibility::Collapsed;
	}
	return ESlateVisibility::Collapsed;
}

void UBFCurrentWeaponWidget::SetConfirmHitVisibility(ESlateVisibility NewVisibility)
{
	ConfirmHitVisibility = NewVisibility;
}

ESlateVisibility UBFCurrentWeaponWidget::GetReloadVisibility() const
{
	ABFPlayerController* OwnerCurrentPlayer = Cast<ABFPlayerController>(GetOwningPlayer());
	if (OwnerCurrentPlayer)
	{
		ABFPlayerCharacter* OwningCurrentCharacter = OwnerCurrentPlayer->GetPoccessedPlayerCharacter();
		if (OwningCurrentCharacter)
		{
			ABFWeaponBase* PlayerCurrentWeapon = OwningCurrentCharacter->GetCurrentWeapon();
			if (PlayerCurrentWeapon)
			{
				if (PlayerCurrentWeapon->GetWeaponState() == EWeaponState::Reload)
				{
					return ESlateVisibility::Visible;
				}
				return ESlateVisibility::Collapsed;
			}
			return ESlateVisibility::Collapsed;
		}
		return ESlateVisibility::Collapsed;
	}
	return ESlateVisibility::Collapsed;
}

FLinearColor UBFCurrentWeaponWidget::GetCrossHairColor() const
{
	ABFPlayerController* OwnerCurrentPlayer = Cast<ABFPlayerController>(GetOwningPlayer());
	if (OwnerCurrentPlayer)
	{
		ABFPlayerCharacter* OwningCurrentCharacter = OwnerCurrentPlayer->GetPoccessedPlayerCharacter();
		if (OwningCurrentCharacter)
		{
			const FItemTraceDetectResult  CurrentTraceDetectResult = OwningCurrentCharacter->GetTraceDetectResult();
			if (CurrentTraceDetectResult.bIsThreat)
			{
				return FLinearColor::Red;
			}
			return FLinearColor::White;
		}
		return FLinearColor::White;
	}
	return FLinearColor::White;
}

FLinearColor UBFCurrentWeaponWidget::GetHitTargetColor() const
{
	ABFPlayerController* OwnerCurrentPlayer = Cast<ABFPlayerController>(GetOwningPlayer());
	if (OwnerCurrentPlayer)
	{
		ABFPlayerCharacter* OwningCurrentCharacter = OwnerCurrentPlayer->GetPoccessedPlayerCharacter();
		if (OwningCurrentCharacter)
		{
			const FTargetHitInfo TargetHitInfo = OwningCurrentCharacter->GetTargetHitInfo();
			//UE_LOG(LogTemp, Warning, TEXT("Hit target in-->victim %s"), TargetHitInfo.bIsTargetDead)
			if (TargetHitInfo.DamgeCause > 0)
			{
				if (TargetHitInfo.bIsTargetDead)
				{
					return FLinearColor::Red;
				}
				return FLinearColor::White;
			}
			return FLinearColor::White;
		}
		return FLinearColor::White;
	}
	return FLinearColor::White;
}

ABFWeaponBase* UBFCurrentWeaponWidget::GetCurrentWeapon()
{
	ABFPlayerController* OwnerCurrentPlayer = Cast<ABFPlayerController>(GetOwningPlayer());
	if (OwnerCurrentPlayer)
	{
		ABFPlayerCharacter* OwningCurrentCharacter = OwnerCurrentPlayer->GetPoccessedPlayerCharacter();
		if (OwningCurrentCharacter)
		{
			CurrentWeapon = OwningCurrentCharacter->GetCurrentWeapon();
			return CurrentWeapon;
		}
		return nullptr;
	}
	return nullptr;
}

int32 UBFCurrentWeaponWidget::GetCurrentAmmo() const
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->GetWeaponData().CurrentClipAmmo;
	}
	return 0;
}

int32 UBFCurrentWeaponWidget::GetLeftAmmo() const
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->GetAmmoLeft();
	}
	return 0;
}

UTexture2D* UBFCurrentWeaponWidget::GetWeaponIConTexture() const
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->WeaponIcon;
	}
	return 0;
}

