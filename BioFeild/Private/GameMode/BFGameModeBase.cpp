// Fill out your copyright notice in the Description page of Project Settings.

#include "BFGameModeBase.h"
#include "Character/BFPlayerCharacter.h"
#include "Bot/BFZombieController.h"
#include "Character/BFPlayerController.h"

ABFGameModeBase::ABFGameModeBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void ABFGameModeBase::ModifyDamage(float& OutDamage, AController*DamageInstigator, AController*Damaged)
{
	/** self damage like gun shot self is not allowed */
	if (DamageInstigator == Damaged)
	{
		OutDamage = 0.f;
	}
}


