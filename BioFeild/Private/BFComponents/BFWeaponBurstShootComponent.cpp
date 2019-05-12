// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeaponBurstShootComponent.h"


UBFWeaponBurstShootComponent::UBFWeaponBurstShootComponent(const FObjectInitializer& ObjectInitialzer) :Super(ObjectInitialzer)
{

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
}


void UBFWeaponBurstShootComponent::SimulateBurstShoot()
{
	const float LastFireTime = WeaponOwner->GetLastFireTime();
	const float GameCurrentTime = WeaponOwner->GetWorld()->GetTimeSeconds();
	if (GameCurrentTime - LastFireTime >= WeaponOwner->GetWeaponData().TimeBetweenShots)
	{
		GetWorld()->GetTimerManager().SetTimer(BurstShootTimerHandle, this, &UBFWeaponBurstShootComponent::EachSingleShoot, WeaponOwner->GetWeaponData().TimeBetweenShots, true, 0.f);
		bNeedIncreaseWeaponSpread = true;
		bNeedRestoreWeaponSpread = false;
		bUseRecoil = true;
	}
}

void UBFWeaponBurstShootComponent::EachSingleShoot()
{
	if (!WeaponOwner->GetCurrentClipAmmo() == 0)
	{
		Super::EachSingleShoot();
	}
	else {
		FinishsBurstShoot();
	}
}

void UBFWeaponBurstShootComponent::FinishsBurstShoot()
{
	GetWorld()->GetTimerManager().ClearTimer(BurstShootTimerHandle);
	bNeedRestoreWeaponSpread = true;
	bNeedIncreaseWeaponSpread = false;
	bUseRecoil = false;
	CurrentRecoilValue = BaseRecoilValue;
}

void UBFWeaponBurstShootComponent::EnableComponentTick()
{
	Super::EnableComponentTick();
}

void UBFWeaponBurstShootComponent::DisableComponentTick()
{
	Super::DisableComponentTick();
}

void UBFWeaponBurstShootComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBFWeaponBurstShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
