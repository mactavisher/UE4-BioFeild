// Fill out your copyright notice in the Description page of Project Settings.

#include "UBFCharacterEnergyComponent.h"
#include "Character/BFBaseCharacter.h"


// Sets default values for this component's properties
UUBFCharacterEnergyComponent::UUBFCharacterEnergyComponent(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	bShouldDecreaseEnergy = false;
	bShouldRestroeEnergy = false;
}


// Called when the game starts
void UUBFCharacterEnergyComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UUBFCharacterEnergyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bShouldRestroeEnergy)
	{
		OnEnergyStartRestore(EnergyDecrement);
	}
	if (bShouldDecreaseEnergy)
	{
		OnEnergyStartDecrease(EnergyDecrement);
	}
}

void UUBFCharacterEnergyComponent::OnEnergyStartDecrease(float DecreaseValue)
{
	CurrentEnergyValue -= DecreaseValue;
	if (CurrentEnergyValue <= 0.f)
	{
		CurrentEnergyValue = MaxEnergyValue;
		OnCharacterExhausted.Broadcast();
		bShouldDecreaseEnergy = false;
	}
}

void UUBFCharacterEnergyComponent::OnEnergyStartRestore(float IncreamentValue)
{
	CurrentEnergyValue += IncreamentValue;
	if (CurrentEnergyValue >= CurrentEnergyValue)
	{
		CurrentEnergyValue = MaxEnergyValue;
		bShouldRestroeEnergy = false;
	}
}

