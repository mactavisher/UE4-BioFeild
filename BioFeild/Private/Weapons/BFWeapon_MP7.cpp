// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeapon_MP7.h"


ABFWeapon_MP7::ABFWeapon_MP7(const FObjectInitializer& ObjectInitailizer) : Super(ObjectInitailizer)
{
	WeaponName = EWeaponNames::MP7;
	WeaponConfigData.TimeBetweenShots = 0.08f;
	WeaponConfigData.AmmoPerClip = 25;
	WeaponConfigData.CurrentClipAmmo = WeaponConfigData.AmmoPerClip;
	WeaponConfigData.BaseDamage = 18.80f;
	WeaponConfigData.MaxAmmo = WeaponConfigData.AmmoPerClip * 10;
	WeaponConfigData.AmmoLeft = WeaponConfigData.MaxAmmo;
	bNeedExtraScopeHolder = false;
}

void ABFWeapon_MP7::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFWeapon_MP7::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ABFWeapon_MP7::BeginPlay()
{
	Super::BeginPlay();
	BurstShootComp->SetBaseRecoilValue(-0.06f);
	BurstShootComp->SetCurrentRecoilValue(BurstShootComp->GetCurrentRecoilValue());
	if (ScopeSlot.AttachmentInstance)
	{
		IronSightMeshComp->SetVisibility(false);
	}
}

void ABFWeapon_MP7::Fire()
{
	Super::Fire();
}

void ABFWeapon_MP7::StopFire()
{
	Super::StopFire();
}

void ABFWeapon_MP7::ToggleFireMode()
{
	Super::ToggleFireMode();
}

FVector ABFWeapon_MP7::GetADSCameraAdjustVector() const
{
	FVector AdjustedVector = FVector::ZeroVector;
	if (!ScopeSlot.AttachmentInstance)
	{
		FVector IronSightADSCameraVector = FVector(30.f, 0.f, -4.8f);
		return IronSightADSCameraVector;
	}
	return  FVector(28.f, 0.f, -4.2f);
}

void ABFWeapon_MP7::SetupAttachments()
{
	Super::SetupAttachments();
	if (ScopeSlot.AttachmentInstance)
	{
		ScopeSlot.AttachmentInstance->AttachToComponent(WeaponMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SightCloserAttachmentPoint");
	}
}

