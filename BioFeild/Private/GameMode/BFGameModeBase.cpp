// Fill out your copyright notice in the Description page of Project Settings.

#include "BFGameModeBase.h"
#include "Character/BFPlayerCharacter.h"
#include "Bot/BFZombieController.h"
#include "DamageTypes/BFDamageType_Falling.h"
#include "Character/BFPlayerController.h"

ABFGameModeBase::ABFGameModeBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bAllowFallingDamage = true;
	bEnableFriendlyFire = false;
}

void ABFGameModeBase::ModifyDamage(float& OutDamage, AController*DamageInstigator, AController*Damaged, FDamageEvent DamageEvent)
{
	/** self damage like gun shot self is not allowed */
	if (DamageInstigator == Damaged)
	{
		OutDamage = 0.f;
		return;
	}
	//const UClass*const DamageTypeClass = DamageEvent.DamageTypeClass->GetClass();
	//if (DamageTypeClass)
	//{
	//	if (DamageTypeClass == UBFDamageType_Falling::StaticClass()&&!bAllowFallingDamage)
	//	{
	//		OutDamage = 0.f;
	//		return;
	//	}
	//}
	//if (DamageTypeClass == UBFProjectileDamage::StaticClass())
	//{

	//}
}


