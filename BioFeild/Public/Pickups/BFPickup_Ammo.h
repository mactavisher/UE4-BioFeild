// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BFPickupBase.h"
#include "BFPickup_Ammo.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFPickup_Ammo : public ABFPickupBase
{
	GENERATED_UCLASS_BODY()

		UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	
};
