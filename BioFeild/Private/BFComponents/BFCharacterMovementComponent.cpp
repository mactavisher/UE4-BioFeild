// Fill out your copyright notice in the Description page of Project Settings.

#include "BFCharacterMovementComponent.h"

UBFCharacterMovementComponent::UBFCharacterMovementComponent(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{

}

void UBFCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	MaxWalkSpeed = 350.f;
	SprintSpeedModifier = 3.f;
	AimingSpeedModifier = 0.6f;
	CrouchSpeedModifier = 0.5f;
	ADSSpeedModifier = 0.4f;
	DefaultMaxWalkSpeed = MaxWalkSpeed;
	BrakingDecelerationWalking = 0.05f;
	MaxAcceleration = 350.f;
}


void UBFCharacterMovementComponent::SetSprint()
{
	MaxAcceleration = 1200.f;
	MaxWalkSpeed = MaxWalkSpeed * SprintSpeedModifier;
}

void UBFCharacterMovementComponent::SetAiming()
{
	MaxWalkSpeed = MaxWalkSpeed * AimingSpeedModifier;
}

void UBFCharacterMovementComponent::SetCrouch()
{
	MaxWalkSpeed = MaxWalkSpeed * CrouchSpeedModifier;
}

void UBFCharacterMovementComponent::SetADS()
{
	MaxWalkSpeed = MaxWalkSpeed * ADSSpeedModifier;
}

void UBFCharacterMovementComponent::SetDefault()
{
	MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void UBFCharacterMovementComponent::SetOwnerCharacter(ABFBaseCharacter* Owner)
{
	OwnerCharacter = Owner;
}



