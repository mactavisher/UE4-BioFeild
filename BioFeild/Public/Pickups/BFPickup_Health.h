// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BFPickupBase.h"
#include "BFPickup_Health.generated.h"

class ABFBaseCharacter;
/**
 * 
 */

UCLASS()
class BIOFEILD_API ABFPickup_Health : public ABFPickupBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		float HealthToProvide;

	virtual void GiveHealthTo(ABFBaseCharacter* BeneficialCharacter);

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
};
