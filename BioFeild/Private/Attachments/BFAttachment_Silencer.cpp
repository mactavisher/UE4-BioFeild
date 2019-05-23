// Fill out your copyright notice in the Description page of Project Settings.

#include "BFAttachment_Silencer.h"

ABFAttachment_Silencer::ABFAttachment_Silencer(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	MuzzleFlashSocket = FName(TEXT("MuzzleFlashSocket"));
	FireLoudnessModifier = 0.01f;
}


