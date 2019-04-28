// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BFComponents/BFWeaponShootComponentBase.h"
#include "BFWeaponTripleShootComponent.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API UBFWeaponTripleShootComponent : public UBFWeaponShootComponentBase
{
	GENERATED_UCLASS_BODY()


		UPROPERTY()
		FTimerHandle TripleShootTimmerHandle;

	UPROPERTY()
		uint8 ShootCount;

	/** start a triple shoot simulate  */
		virtual void SimulateTripleShoot();

		/** override function to extends */
		virtual void EachSingleShoot()override;

		/** finish pipeline */
		virtual void FinishTripleShoot();

};
