// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeaponSingleShootComponent.h"
#include"Engine/World.h"

UBFWeaponSingleShootComponent::UBFWeaponSingleShootComponent(const FObjectInitializer&ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UBFWeaponSingleShootComponent::ShootCoolDown()
{ 
	bNeedRestoreWeaponSpread = true;
	bNeedIncreaseWeaponSpread = false;
	bUseRecoil = false;
	CurrentRecoilValue = BaseRecoilValue;
}

void UBFWeaponSingleShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBFWeaponSingleShootComponent::SimulateSingleShoot()
{
	const float LastFireTime = WeaponOwner->GetLastFireTime();
	const float GameCurrentTime = WeaponOwner->GetWorld()->GetTimeSeconds();
	if (GameCurrentTime-LastFireTime>= WeaponOwner->GetWeaponData().TimeBetweenShots)
	{
		EachSingleShoot();
		bNeedRestoreWeaponSpread = false;
		bNeedIncreaseWeaponSpread = true;
		bUseRecoil = true;
		GetWorld()->GetTimerManager().SetTimer(SingleShotCoolDownHandle, this, &UBFWeaponSingleShootComponent::ShootCoolDown, 1.0f, false, 0.125f);
	}
}

 void UBFWeaponSingleShootComponent::BeginPlay()
{
	Super::BeginPlay();
}

