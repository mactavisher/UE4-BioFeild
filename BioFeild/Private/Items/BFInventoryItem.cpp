// Fill out your copyright notice in the Description page of Project Settings.

#include "BFInventoryItem.h"
#include "Character/BFBaseCharacter.h"
#include "Character/BFPlayerController.h"

ABFInventoryItem::ABFInventoryItem(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

void ABFInventoryItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABFInventoryItem::ReceiveDetected(class AActor* DetectedBy, class ABFBaseCharacter* DectectedCharacter, class ABFPlayerController* DectedPlayer)
{
	const FVector CharacterLocation = DectectedCharacter->GetActorLocation();
	const FVector SelfLocation = this->GetActorLocation();
	const float Distance = FVector::Distance(SelfLocation, CharacterLocation);
	if (FMath::Abs(Distance) <= 500.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("You are close enough to pick me up"));
	}
}


