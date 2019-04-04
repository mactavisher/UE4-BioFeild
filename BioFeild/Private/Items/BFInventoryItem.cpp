// Fill out your copyright notice in the Description page of Project Settings.

#include "BFInventoryItem.h"
#include "Character/BFBaseCharacter.h"
#include "Character/BFPlayerController.h"

ABFInventoryItem::ABFInventoryItem(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABFInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABFInventoryItem::ReceiveDetected(class AActor* DetectedBy, class ABFBaseCharacter* DectectedCharacter, class ABFPlayerController* DectedPlayer)
{

}

void ABFInventoryItem::NotifyReaction(class AActor* NotifiedActor)
{

}

