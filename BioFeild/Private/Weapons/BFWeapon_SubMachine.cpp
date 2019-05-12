// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeapon_SubMachine.h"
#include "BFComponents/BFWeaponBurstShootComponent.h"

ABFWeapon_SubMachine::ABFWeapon_SubMachine(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	BurstShootComp = ObjectInitializer.CreateDefaultSubobject<UBFWeaponBurstShootComponent>(this, TEXT("BurstShotComp"));
}

void ABFWeapon_SubMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFWeapon_SubMachine::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABFWeapon_SubMachine::BeginPlay()
{
	Super::BeginPlay();
	BurstShootComp->SetWeaponOwner(this);
	BurstShootComp->DisableComponentTick();
}

void ABFWeapon_SubMachine::Fire()
{
	BurstShootComp->SimulateBurstShoot();
}

void ABFWeapon_SubMachine::StopFire()
{
	BurstShootComp->FinishsBurstShoot();
}

void ABFWeapon_SubMachine::ToggleFireMode()
{
	return;
}

void ABFWeapon_SubMachine::OnWeaponEquipingFinished()
{
	Super::OnWeaponEquipingFinished();
	BurstShootComp->EnableComponentTick();
}

void ABFWeapon_SubMachine::OnWeaponUnequipingFinished()
{
	Super::OnWeaponUnequipingFinished();
	BurstShootComp->DisableComponentTick();
}

