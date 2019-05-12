// Fill out your copyright notice in the Description page of Project Settings.

#include "BFCharacterWidget.h"
#include "Character/BFPlayerCharacter.h"
UBFCharacterWidget::UBFCharacterWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

float UBFCharacterWidget::GetHealthPercentage() const
{
	ABFPlayerController* OwningPlayer = Cast<ABFPlayerController>(GetOwningPlayer());
	if (OwningPlayer)
	{
		ABFPlayerCharacter* OwningCharacter = OwningPlayer->GetPoccessedPlayerCharacter();
		return OwningCharacter->GetHealthPercentage();
	}
	return 0.f;
}

