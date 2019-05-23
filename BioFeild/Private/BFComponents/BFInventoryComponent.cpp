// Fill out your copyright notice in the Description page of Project Settings.

#include "BFInventoryComponent.h"
#include "Weapons/BFWeaponBase.h"
#include "Character/BFPlayerCharacter.h"
#include "Engine/World.h"
#include "Weapons/BFWeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "BFComponents/BFWeaponMeshComponent.h"

UBFInventoryComponent::UBFInventoryComponent(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBFInventoryComponent::AddWeaponToSlot(ABFWeaponBase* WeaponToAdd)
{
	for (uint8 i = 0; i <= WeaponSlots.Num() - 1; i++)
	{
		if (!WeaponSlots[i].bIsSlotOccupied)
		{
			WeaponSlots[i].SlotWeapon = WeaponToAdd;
			WeaponSlots[i].bIsSlotOccupied = true;
			break;
		}
	}
}

void UBFInventoryComponent::AddWeaponToSpecificSlot(ABFWeaponBase* WeaponToAdd, uint8 SlotIndex)
{
	if (WeaponSlots.Num() >=(SlotIndex+1))
	{
		if (!WeaponSlots[SlotIndex].bIsSlotOccupied)
		{
			WeaponSlots[SlotIndex].SlotWeapon = WeaponToAdd;
			WeaponSlots[SlotIndex].bIsSlotOccupied = true;
			WeaponSlots[SlotIndex].SlotWeapon->SetWeaponSlotIndex(SlotIndex);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("slot is used ,please choose another slot "));
	}
}

ABFWeaponBase* UBFInventoryComponent::GiveSlotWeapon(uint8 SlotIndex)
{
	if (WeaponSlots.Num()>=(SlotIndex+1))
	{
		if (WeaponSlots[SlotIndex].SlotWeapon && !WeaponSlots[SlotIndex].SlotWeapon->GetIsSelectedAsCurrent())
		{
			return WeaponSlots[SlotIndex].SlotWeapon;
		}
	}
	return nullptr;
}

void UBFInventoryComponent::RemoveSlotWeapon(uint8 SlotIndex)
{
	if (WeaponSlots.Num() - 1 >= SlotIndex+1)
	{
		WeaponSlots[SlotIndex].SlotWeapon = nullptr;
		WeaponSlots[SlotIndex].bIsSlotOccupied = false;
	}
}

void UBFInventoryComponent::initializeAvailableWeapon()
{
	for (uint8 i = 0; i <= WeaponSlots.Num() - 1; i++)
	{
		if (WeaponSlots[i].InitialSlotWeaponClass)
		{
			WeaponSlots[i].SlotWeapon = GetWorld()->SpawnActorDeferred<ABFWeaponBase>(WeaponSlots[i].InitialSlotWeaponClass, OwnerPlayerCharacter->GetActorTransform(), OwnerPlayerCharacter, OwnerPlayerCharacter, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			WeaponSlots[i].SlotWeapon->SetWeaponOwner(OwnerPlayerCharacter);
			WeaponSlots[i].bIsSlotOccupied = true;
			WeaponSlots[i].SlotWeapon->SetWeaponSlotIndex(i);
		}
	}
}

ABFWeaponBase* UBFInventoryComponent::GiveDefaultWeapon()
{
	ABFWeaponBase* WeaponToGive = nullptr;
	const uint8 WeaponSlotsNum = WeaponSlots.Num();
	for (uint8 i = 0; i <= WeaponSlotsNum - 1; i++)
	{
		if (WeaponSlots[i].SlotWeapon)
		{
			WeaponToGive = WeaponSlots[i].SlotWeapon;
			break;
		}
	}
	return WeaponToGive;
}

void UBFInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}
