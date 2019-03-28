// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPickup_Ammo.h"
#include "Character/BFBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Character/BFPlayerCharacter.h"
#include "BFComponents/BFInventoryComponent.h"
#include "Weapons/BFWeaponBase.h"
ABFPickup_Ammo::ABFPickup_Ammo(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void ABFPickup_Ammo::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABFPlayerCharacter* Player = Cast<ABFPlayerCharacter>(OtherActor);
	if (Player)
	{
		UBFInventoryComponent* InventoryComp = Player->InventoryComponent;
		/*	TArray<ABFWeaponBase*> Inventory = InventoryComp->GetInventory();
			if (Inventory.Num() >= 0)
			{
				for (int32 i = 0; i < Inventory.Num(); i++)
				{
					ABFWeaponBase* Weapon = Inventory[i];
					const float AmmoPerClip = Weapon->GetWeaponData().AmmoPerClip;
					Weapon->SetAmmoLeft(AmmoPerClip * 2);
				}
				bIsActive = false;
				bIsExpired = true;*/
			Destroy(true);
		//}
	}
}



