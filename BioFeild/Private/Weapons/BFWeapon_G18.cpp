// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeapon_G18.h"

ABFWeapon_G18::ABFWeapon_G18(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
	WeaponName = EWeaponNames::CLOCK18;
	WeaponConfigData.TimeBetweenShots = 0.15f;
	WeaponConfigData.AmmoPerClip = 14;
	WeaponConfigData.CurrentClipAmmo = WeaponConfigData.AmmoPerClip;
	WeaponConfigData.BaseDamage = 28.38f;
	WeaponConfigData.MaxAmmo = WeaponConfigData.AmmoPerClip * 4;
	WeaponConfigData.AmmoLeft = WeaponConfigData.MaxAmmo;
	bNeedExtraScopeHolder = false;
}


void ABFWeapon_G18::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFWeapon_G18::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABFWeapon_G18::BeginPlay()
{
	Super::BeginPlay();
	SingleShotComp->SetBaseRecoilValue(-0.07f);
}

void ABFWeapon_G18::Fire()
{
	Super::Fire();
}

void ABFWeapon_G18::StopFire()
{
	Super::StopFire();
}

void ABFWeapon_G18::ToggleFireMode()
{
	//this gun only has a single fire mode , nothing to do with toggle fire mode
	return;
}

FVector ABFWeapon_G18::GetADSCameraAdjustVector() const
{
	if (ScopeSlot.AttachmentInstance)
	{
		return FVector(25.f,0.f,-1.25f);
	}
	return FVector(25.f, 0, -5.25f);
}

void ABFWeapon_G18::SetupAttachments()
{
	Super::SetupAttachments();
}
