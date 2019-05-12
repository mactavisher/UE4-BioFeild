// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeapon_Scar.h"

ABFWeapon_Scar::ABFWeapon_Scar(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	WeaponConfigData.AmmoPerClip = 30;
	WeaponConfigData.BaseDamage = 32.f;
	WeaponConfigData.MaxAmmo = 90;
	WeaponConfigData.TimeBetweenShots = 0.115f;
	WeaponName = EWeaponNames::SCAR;
}

void ABFWeapon_Scar::BeginPlay()
{
	Super::BeginPlay();
	BurstShootComp->SetBaseRecoilValue(-0.085f);
	SingleShootComp->SetBaseRecoilValue(-0.25f);
}

void ABFWeapon_Scar::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

FVector ABFWeapon_Scar::GetADSCameraAdjustVector() const
{
	FVector AdjustedVector = FVector::ZeroVector;
	if (!ScopeSlot.AttachmentInstance)
	{
		FVector IronSightADSCameraVector = FVector(60.f, 0.f, -4.8f);
		return IronSightADSCameraVector;
	}
	return  FVector(30.f, 0.f, -4.8f);
}

