// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BFBaseCharacter.h"
#include "Character/BFBaseCharacter.h"
#include "Character/BFPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Items/BFInventoryItem.h"
#include "Animation/BFAnimInstance.h"
#include "Components/TimelineComponent.h"
#include "BFPlayerCharacter_FPS.generated.h"


class ABFWeaponBase;
class UBFInventoryComponent;
class ABFPlayerController;
class UBFAnimInstance;
class UPawnNoiseEmitterComponent;

/** define a character's weapon Action Type */
namespace ECharacterWeaponAction_FPS
{
	enum Type
	{
		Idle,
		Equiping,
		UnEquiping,
		Firing,
	};
}
/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFPlayerCharacter_FPS : public ABFBaseCharacter
{
	GENERATED_UCLASS_BODY()


	
	
	
	
};
