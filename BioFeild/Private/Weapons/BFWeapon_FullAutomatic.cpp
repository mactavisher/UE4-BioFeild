// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeapon_FullAutomatic.h"
#include "BFComponents/BFWeaponSingleShootComponent.h"
#include "BFComponents/BFWeaponBurstShootComponent.h"
ABFWeapon_FullAutomatic::ABFWeapon_FullAutomatic(const FObjectInitializer&ObjectInitializer) :Super(ObjectInitializer)
{
	SingleShootComp = ObjectInitializer.CreateDefaultSubobject<UBFWeaponSingleShootComponent>(this, TEXT("SingleShootComp"));
	BurstShootComp = ObjectInitializer.CreateDefaultSubobject<UBFWeaponBurstShootComponent>(this, TEXT("BurstShootComp"));
	PrimaryActorTick.bCanEverTick = true;
	FireMode = EFireMode::BurstShot;
	AdjustADSCameraVector = FVector::ZeroVector;
}

void ABFWeapon_FullAutomatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFWeapon_FullAutomatic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABFWeapon_FullAutomatic::BeginPlay()
{
	Super::BeginPlay();
	SingleShootComp->RegisterComponentWithWorld(GetWorld());
	BurstShootComp->RegisterComponentWithWorld(GetWorld());
	SingleShootComp->SetWeaponOwner(this);
	BurstShootComp->SetWeaponOwner(this);
	SingleShootComp->DisableComponentTick();
	BurstShootComp->EnableComponentTick();
}

void ABFWeapon_FullAutomatic::Fire()
{
	if (FireMode == EFireMode::BurstShot)
	{
		BurstShootComp->SimulateBurstShoot();
	}
	else if (FireMode == EFireMode::SingleShot)
	{
		SingleShootComp->SimulateSingleShoot();
	}
}

void ABFWeapon_FullAutomatic::StopFire()
{
	if (FireMode == EFireMode::BurstShot)
	{
		BurstShootComp->FinishsBurstShoot();
	}
}

void ABFWeapon_FullAutomatic::ToggleFireMode()
{
	if (FireMode == EFireMode::BurstShot)
	{
		FireMode = EFireMode::SingleShot;
		BurstShootComp->DisableComponentTick();
		SingleShootComp->EnableComponentTick();
	}
	else if (FireMode == EFireMode::SingleShot)
	{
		FireMode = EFireMode::BurstShot;
	    SingleShootComp ->DisableComponentTick();
		BurstShootComp->EnableComponentTick();
	}
}

