// Fill out your copyright notice in the Description page of Project Settings.

#include "BFAnimInstance.h"
#include "Character/BFPlayerCharacter.h"
#include "BFComponents/BFSkeletalMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFComponents/BFCharacterMovementComponent.h"

UBFAnimInstance::UBFAnimInstance(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
 
}


void UBFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Character&&CharacterMovementComp)
	{
		bIsIdle = Character->GetVelocity().Size() == 0.f;
		bIsInAir = CharacterMovementComp->IsFalling();
		bCanWalking = (FMath::Abs(CharacterMovementComp->GetLastInputVector().Size()) > 0.f)&&!bIsInAir;
		bCanStopWalking = CharacterMovementComp->GetLastInputVector().Size() == 0.f;
		PitchOffset = CharacterSkeletalMesh->CalculatePitchOffSet();
		Direction = CalculateDirection();
		SpeedXY = GetXYSpeed();
		bIsWalking =SpeedXY> CharacterMovementComp->MaxWalkSpeed * 0.9f;
		bIsSprinting = Character->bIsSprint;
    }
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UBFAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ABFPlayerCharacter>(TryGetPawnOwner());
	if (Character)
	{
		CharacterMovementComp = Character->GetBFCharacterMovement();
		CharacterSkeletalMesh = Character->GetBFSkeletalMesh();
	}
}

float UBFAnimInstance::CalculateDirection()
{
	const FVector LastInputVector = CharacterMovementComp->GetLastInputVector();
	const FRotator CharacterRotation = Character->GetActorRotation();
	return Super::CalculateDirection(LastInputVector, CharacterRotation);
}

float UBFAnimInstance::GetXYSpeed()
{
	const FVector CurrentVelocity = Character->GetVelocity();
	return FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.f).Size();
}
