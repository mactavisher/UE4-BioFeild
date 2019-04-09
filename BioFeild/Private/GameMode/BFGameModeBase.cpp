// Fill out your copyright notice in the Description page of Project Settings.

#include "BFGameModeBase.h"
#include "Character/BFPlayerCharacter.h"
#include "Bot/BFZombieController.h"
#include "Character/BFPlayerController.h"

ABFGameModeBase::ABFGameModeBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	TeamDamageModifier = 0.2f;
}

void ABFGameModeBase::ModifyDamage(float& OutDamage, AController* DamageCauser, AController*DamagedPlayer)
{
	/** self damage like gun shot self is not allowed */
	if (DamagedPlayer == DamageCauser)
	{
		OutDamage = 0.f;
	}
	if (DamageCauser&&DamagedPlayer)
	{
		if (DamageCauser->GetClass() == DamagedPlayer->GetClass())
		{
			const ABFPlayerController* DamageCauserPlayer = Cast<ABFPlayerController>(DamageCauser);
			const ABFPlayerController* Damaged = Cast<ABFPlayerController>(DamagedPlayer);
			if (DamageCauserPlayer->GetTeam() == Damaged->GetTeam())
			{
				OutDamage = OutDamage * TeamDamageModifier;
			}
		}
		if (DamageCauser->GetClass()->IsChildOf(ABFPlayerController::StaticClass())&& DamagedPlayer->GetClass()->IsChildOf(ABFZombieController::StaticClass()))
		{
			OutDamage = OutDamage;
		}
	}
}


