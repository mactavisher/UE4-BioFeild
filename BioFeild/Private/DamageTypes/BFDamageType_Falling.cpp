// Fill out your copyright notice in the Description page of Project Settings.

#include "BFDamageType_Falling.h"




UBFDamageType_Falling::UBFDamageType_Falling(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	DamageMessage = "you are taking falling damage";
	KillMessage = "You are killed by falling";
}