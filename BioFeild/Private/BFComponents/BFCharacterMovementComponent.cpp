// Fill out your copyright notice in the Description page of Project Settings.

#include "BFCharacterMovementComponent.h"

UBFCharacterMovementComponent::UBFCharacterMovementComponent(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	MaxWalkSpeed = 350.f;
	SprintSpeedModifier = 3.f;
	AimingSpeedModifier = 0.6f;
	CrouchSpeedModifier = 0.5f;
	ADSSpeedModifier = 0.4f;
	DefaultMaxWalkSpeed = MaxWalkSpeed;
	BrakingDecelerationWalking = 0.05f;
	MaxAcceleration = 350.f;
}

void UBFCharacterMovementComponent::SetSprintSpeed()
{
	MaxAcceleration = 600.f;
	MaxWalkSpeed = MaxWalkSpeed* SprintSpeedModifier;
}

void UBFCharacterMovementComponent::SetAimingSpeed()
{
	MaxWalkSpeed = MaxWalkSpeed* AimingSpeedModifier;
}

void UBFCharacterMovementComponent::SetCrouchSpeed()
{
	MaxWalkSpeed = MaxWalkSpeed* CrouchSpeedModifier;
}

void UBFCharacterMovementComponent::SetADSSpeed()
{
	MaxWalkSpeed = MaxWalkSpeed* ADSSpeedModifier;
}

void UBFCharacterMovementComponent::SetDefaultMaxWalkSpeed()
{
	MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void UBFCharacterMovementComponent::SetOwnerCharacter(ABFBaseCharacter* Owner)
{
	OwnerCharacter = Owner;
}



