// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeaponTripleShootComponent.h"


UBFWeaponTripleShootComponent::UBFWeaponTripleShootComponent(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ShootCount = 0;
}

void UBFWeaponTripleShootComponent::SimulateTripleShoot()
{
	const FWeaponConfigData WeaponConfig = WeaponOwner->GetWeaponData();
	GetWorld()->GetTimerManager().SetTimer(TripleShootTimmerHandle, this, &UBFWeaponTripleShootComponent::EachSingleShoot, true, 0.f);
}

void UBFWeaponTripleShootComponent::EachSingleShoot()
{
	Super::EachSingleShoot();
	ShootCount++;
	//if 3 shoot finished , then clear the timer,or weapon can't satisfy to finish 3 shoot , clear the timer
	if (ShootCount == 3||(ShootCount < 3 && WeaponOwner->GetCurrentClipAmmo() == 0))
	{
		FinishTripleShoot();
	}
}

void UBFWeaponTripleShootComponent::FinishTripleShoot()
{
	GetWorld()->GetTimerManager().ClearTimer(TripleShootTimmerHandle);
	bUseRecoil = false;
	bNeedRestoreWeaponSpread = true;
	bNeedIncreaseWeaponSpread = false;
	ShootCount = 0;//reset this value to 0;
}

