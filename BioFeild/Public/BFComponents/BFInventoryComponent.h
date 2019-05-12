// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/BFUserWidgetBase.h"
#include "BFInventoryComponent.generated.h"

class ABFWeaponBase;
class ABFPlayerCharacter;
class UBFUserWidgetBase;

USTRUCT(BlueprintType)
struct FWeaponSlot {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SlotName")
		FName SlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFWeapon")
		TSubclassOf<class ABFWeaponBase> InitialSlotWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BFWeapon")
		ABFWeaponBase* SlotWeapon;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFWeapon")
		uint8 bIsSlotOccupied : 1;
	//default values
		FWeaponSlot()
	{
		bIsSlotOccupied = false;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOFEILD_API UBFInventoryComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|UI")
		TSubclassOf<UBFUserWidgetBase> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
		TArray<FWeaponSlot> WeaponSlots;

private:

	UPROPERTY(VisibleDefaultsOnly,Category ="Inventory")
		ABFPlayerCharacter* OwnerPlayerCharacter;

	UPROPERTY()
		UBFUserWidgetBase* InventoryWidgetInstance;

public:
	virtual uint8 GetWeaponSlotSize() const { return WeaponSlots.Num(); }

	virtual void  GetWeaponSlot(TArray<FWeaponSlot>& OutWeaponSlots) { OutWeaponSlots = WeaponSlots; };

    /** automatically add weapon to  a empty slot */
	virtual void AddWeaponToSlot(ABFWeaponBase* WeaponToAdd);

	virtual void AddWeaponToSpecificSlot(ABFWeaponBase* WeaponToAdd, uint8 SlotIndex);

	virtual ABFWeaponBase* GiveSlotWeapon(uint8 SlotIndex);

	virtual void SetOwnerCharacter(ABFPlayerCharacter* OwnerPlayerCharacter) { this->OwnerPlayerCharacter = OwnerPlayerCharacter; }

	virtual ABFPlayerCharacter* GetOwnerPlayerCharacter()const { return OwnerPlayerCharacter; }

	virtual  void RemoveSlotWeapon(uint8 SlotIndex);

	virtual void initializeAvailableWeapon();

	virtual ABFWeaponBase* GiveDefaultWeapon();

	virtual void SetInventoryWidget(UBFUserWidgetBase* InWidget) { this->InventoryWidgetInstance = InWidget; };

	virtual UBFUserWidgetBase* GetInventoryWidget()const { return InventoryWidgetInstance; };

protected:
	virtual void BeginPlay()override;
};
