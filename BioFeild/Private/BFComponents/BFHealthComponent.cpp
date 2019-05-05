// Fill out your copyright notice in the Description page of Project Settings.

#include "BFHealthComponent.h"
#include "Character/BFBaseCharacter.h"
#include "BFComponents/BFCharacterAudioComponent.h"

UBFHealthComponent::UBFHealthComponent(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
	DefaultHealth = 100.f;
	MaximunHealth = 200.f;
	CurrentHealth = DefaultHealth;
	LowHealthPercentage = 0.5f;
}

float UBFHealthComponent::GetHealthPercentage()
{
	float percentage = CurrentHealth / DefaultHealth;
	return percentage;
}

bool UBFHealthComponent::CheckIsLowHealth()
{
	return ((CurrentHealth / DefaultHealth) <= LowHealthPercentage);
}

void UBFHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBFHealthComponent::SetHealth(float Health)
{
	if (!OwnerCharacter->GetCharacterIsDead())
	{
		this->CurrentHealth = Health;
		if (CurrentHealth <= 0)
		{
			CurrentHealth = 0;
			OnCharacterShouldDie.Broadcast();
		}
	}
}

void UBFHealthComponent::ReduceHealth(float ReduceAmount)
{
	if (!OwnerCharacter->GetCharacterIsDead()&&ReduceAmount>0.f)
	{
		CurrentHealth = CurrentHealth - ReduceAmount;
		if (CurrentHealth > 0.f)
		{
			OnHealthReduced.Broadcast(ReduceAmount);
		}
		//make sure that character's current health won't go below 0
		if (CheckIsLowHealth() && CurrentHealth >= 0)
		{
			OnLowHealth.Broadcast();
		}
		if (CurrentHealth <= 0.f)
		{
			CurrentHealth = 0.f;
			OnCharacterShouldDie.Broadcast();
		}
	}
}

void UBFHealthComponent::ReGenerateHealth(float GenerateAmount)
{
	//make sure that character's current health won't go above MaximunHealth
	FMath::Clamp(CurrentHealth += GenerateAmount, 0.f, DefaultHealth);
	if (CurrentHealth >= MaximunHealth)
	{
		CurrentHealth = MaximunHealth;
	}
}