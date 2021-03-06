// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeapon_AK47.h"

ABFWeapon_AK47::ABFWeapon_AK47(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	AdjustADSCameraVector = FVector(21.f, 0.f, 20.25f);
	IronSightMeshComp->SetVisibility(false);
	WeaponConfigData.TimeBetweenShots = 0.125f;
	WeaponConfigData.AmmoPerClip = 31;
	WeaponConfigData.CurrentClipAmmo = WeaponConfigData.AmmoPerClip;
	WeaponConfigData.BaseDamage = 35.64f;
	WeaponConfigData.MaxAmmo = WeaponConfigData.AmmoPerClip * 4;
	WeaponConfigData.AmmoLeft = WeaponConfigData.MaxAmmo;
	bNeedExtraIronSight = false;
	bNeedExtraScopeHolder = true;
}

void ABFWeapon_AK47::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFWeapon_AK47::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABFWeapon_AK47::BeginPlay()
{
	if (!ScopeSlot.AttachmentInstance)
	{
		ScopeHolderMeshComp->SetVisibility(false);
	}
	Super::BeginPlay();
}

void ABFWeapon_AK47::Fire()
{
	Super::Fire();
}

void ABFWeapon_AK47::StopFire()
{
	Super::StopFire();
}

void ABFWeapon_AK47::ToggleFireMode()
{
	Super::ToggleFireMode();
}

FVector ABFWeapon_AK47::GetADSCameraAdjustVector()const
{
	FVector AdjustedVector = FVector::ZeroVector;
	if (!ScopeSlot.AttachmentInstance)
	{
		FVector IronSightADSCameraVector = FVector(35.f, -0.2f, -3.8f);
		return IronSightADSCameraVector;
	}
	return  FVector(25.f, 0.f, 0.2f);
}

