// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPainCausingVolume.h"
#include "Character/BFPlayerCharacter.h"


ABFPainCausingVolume::ABFPainCausingVolume(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bCausingPainToPlayerCharacter = true;
}

void ABFPainCausingVolume::CausePainTo(class AActor* Other)
{
	const uint8 bIsPlayerCharacter = Other->GetClass()->IsChildOf(ABFPlayerCharacter::StaticClass());
	const uint8 bIsCharacterType = Other->GetClass()->IsChildOf(ABFBaseCharacter::StaticClass());
	if (bIsCharacterType)
	{
		if (bIsPlayerCharacter)
		{
			if (bCausingPainToPlayerCharacter)
			{
				Super::CausePainTo(Other);
			}
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("ingore pain causing to player characters!!"));
#endif
		}
	}
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("ingore pain causing to player character"));
#endif
}
