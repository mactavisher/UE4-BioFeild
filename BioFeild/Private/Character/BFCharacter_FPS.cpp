// Fill out your copyright notice in the Description page of Project Settings.

#include "BFCharacter_FPS.h"

ABFCharacter_FPS::ABFCharacter_FPS(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	CameraArmComp->TargetArmLength = 0.f;
	
}

void ABFCharacter_FPS::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ABFCharacter_FPS::BeginPlay()
{
	UBFSkeletalMeshComponent* FPSCharacterMesh = Cast<UBFSkeletalMeshComponent>(GetMesh());
	if (FPSCharacterMesh)
	{
		FPSCharacterMesh->AttachToComponent(CameraComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void ABFCharacter_FPS::Tick(float DeltaTimeSeconds)
{

}

