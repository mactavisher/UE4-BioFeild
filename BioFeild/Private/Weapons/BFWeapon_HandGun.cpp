// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeapon_HandGun.h"

ABFWeapon_HandGun::ABFWeapon_HandGun(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	SingleShotComp = ObjectInitializer.CreateDefaultSubobject<UBFWeaponSingleShootComponent>(this, TEXT("SingleShootComp"));
}

void ABFWeapon_HandGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFWeapon_HandGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABFWeapon_HandGun::BeginPlay()
{
	Super::BeginPlay();
	SingleShotComp->SetWeaponOwner(this);

	//turn off tick by default, until this weapon is equipped by player character
	SingleShotComp->DisableComponentTick();
}

void ABFWeapon_HandGun::Fire()
{
	SingleShotComp->SimulateSingleShoot();
}

void ABFWeapon_HandGun::StopFire()
{
	Super::StopFire();
}

void ABFWeapon_HandGun::ToggleFireMode()
{
	return;
}

FVector ABFWeapon_HandGun::GetADSCameraAdjustVector() const
{
	return FVector(25.f, 0, -5.25f);
}

void ABFWeapon_HandGun::SetupAttachments()
{
	Super::SetupAttachments();
}

void ABFWeapon_HandGun::OnWeaponEquipingFinished()
{
	Super::OnWeaponEquipingFinished();
	SingleShotComp->EnableComponentTick();
}

void ABFWeapon_HandGun::OnWeaponUnequipingFinished()
{
	Super::OnWeaponUnequipingFinished();
	SingleShotComp->DisableComponentTick();
}

