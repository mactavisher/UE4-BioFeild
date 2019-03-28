// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"
#include "Character/BFPlayerCharacter.h"

ABFPlayerController::ABFPlayerController(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	GlobalTimeDialationAmount = 0.15f;
#if WITH_EDITORONLY_DATA
	ArrowComp = ObjectInitailizer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("Arrow Comp"));
	ArrowComp->SetupAttachment(RootComponent);
	ArrowComp->SetArrowColor(FLinearColor::Green);
	ArrowComp->bIsScreenSizeScaled = true;
	ArrowComp->bTreatAsASprite = true;
	ArrowComp->SetVisibility(true);
#endif
}

void ABFPlayerController::BeginPlay()
{

}

void ABFPlayerController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);
	PoccessedPlayerCharacter = Cast<ABFPlayerCharacter>(Pawn);
	PoccessedPlayerCharacter->SetPlayerController(this);
}

void ABFPlayerController::UnPossess()
{

}

void ABFPlayerController::AddScore(int32 Score)
{
	this->Score += Score;
}


void ABFPlayerController::EnableSlowMotion()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), GlobalTimeDialationAmount);
}


void ABFPlayerController::DisableSlowMotion()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
