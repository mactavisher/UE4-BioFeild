// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeaponShootComponentBase.h"
#include "Weapons/BFWeaponBase.h"
#include "Character/BFPlayerCharacter.h"
#include "BFComponents/BFSkeletalMeshComponent.h"

// Sets default values for this component's properties
UBFWeaponShootComponentBase::UBFWeaponShootComponentBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	bNeedRestoreWeaponSpread = false;
	bNeedIncreaseWeaponSpread = false;
	bUseRecoil = false;
	BaseRecoilValue = -0.08;
	// ...
}


void UBFWeaponShootComponentBase::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UBFWeaponShootComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bNeedRestoreWeaponSpread)
	{
		DecreaseWeaponSpread(DeltaTime*50.f);
	}
	if (bNeedIncreaseWeaponSpread)
	{
		IncreaseWeaponSpread(DeltaTime*10.f);
	}
	if (bUseRecoil)
	{
		//@TO-DO this recoil value should be configurable
		WeaponOwner->RecoilPlayer(CurrentRecoilValue);
		CurrentRecoilValue = CurrentRecoilValue + 0.0003f;
		if (CurrentRecoilValue >0.f)
		{
			CurrentRecoilValue = 0.f;
		}
#if WITH_EDITOR
		FString Debugmessage;
		Debugmessage.Append(TEXT("weapon current recoil value")).Append(FString::SanitizeFloat(CurrentRecoilValue));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Debugmessage);
#endif
	}
}

void UBFWeaponShootComponentBase::EnableComponentTick()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UBFWeaponShootComponentBase::DisableComponentTick()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	if (WeaponOwner)
	{
		FWeaponSpreadData CurrenetWeaponSpreadData = WeaponOwner->GetWeaponSpreadData();
		WeaponOwner->SetCurrentWeaponSpread(0.f);
		bNeedRestoreWeaponSpread = false;
		bNeedIncreaseWeaponSpread = false;
		bUseRecoil = false;
		CurrentRecoilValue = BaseRecoilValue;//reset recoil value 
	}
}

void UBFWeaponShootComponentBase::EachSingleShoot()
{
	if (WeaponOwner)
	{
		WeaponOwner->SingleShot();
	}
}

void UBFWeaponShootComponentBase::IncreaseWeaponSpread(float Value)
{
	if (WeaponOwner)
	{
		FWeaponSpreadData CurrentWeaponSpreadData = WeaponOwner->GetWeaponSpreadData();
		if (CurrentWeaponSpreadData.CurrentWeaponSpread < CurrentWeaponSpreadData.WeaponSpreadMax)
		{
			WeaponOwner->SetCurrentWeaponSpread(WeaponOwner->GetCurrentWeaponSpread() + Value);
			// make sure weapon current Weapon spread will not go beyond max spread limit
			if (WeaponOwner->GetCurrentWeaponSpread() >= CurrentWeaponSpreadData.WeaponSpreadMax)
			{
				WeaponOwner->SetCurrentWeaponSpread(CurrentWeaponSpreadData.WeaponSpreadMax);
				bNeedIncreaseWeaponSpread = false;
			}
		}
//#if WITH_EDITOR
//		FString Debugmessage;
//		Debugmessage.Append(TEXT("weapon current spread value is :")).Append(FString::SanitizeFloat(CurrentWeaponSpreadData.CurrentWeaponSpread));
//		GEngine->AddOnScreenDebugMessage(-1, 0.8f, FColor::Red, Debugmessage);
//#endif
	}
}

void UBFWeaponShootComponentBase::DecreaseWeaponSpread(float Value)
{
	if (WeaponOwner)
	{
		FWeaponSpreadData CurrentWeaponSpreadData = WeaponOwner->GetWeaponSpreadData();
		if (WeaponOwner->GetCurrentWeaponSpread() > 0)
		{
			WeaponOwner->SetCurrentWeaponSpread(WeaponOwner->GetCurrentWeaponSpread()-Value);

			// make sure weapon current Weapon spread will not go below max spread limit
			if (WeaponOwner->GetCurrentWeaponSpread() <= 0.f)
			{
				WeaponOwner->SetCurrentWeaponSpread(0.f);
				bNeedRestoreWeaponSpread = false;
			}
		}
//#if WITH_EDITOR
//		FString Debugmessage;
//		Debugmessage.Append(TEXT("weapon current spread value is :")).Append(FString::SanitizeFloat(CurrentWeaponSpreadData.CurrentWeaponSpread));
//		GEngine->AddOnScreenDebugMessage(-1, 0.8f, FColor::Red, Debugmessage);
//#endif
	}
}

