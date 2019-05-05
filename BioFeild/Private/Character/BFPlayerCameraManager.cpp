// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPlayerCameraManager.h"
#include "Character/BFPlayerController.h"
#include "Character/BFPlayerCharacter.h"

ABFPlayerCameraManager::ABFPlayerCameraManager(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABFPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
		BFPlayerController = Cast<ABFPlayerController>(GetOwningPlayerController());
		if (BFPlayerController)
		{
			ABFPlayerCharacter* PlayerCharacter = BFPlayerController->GetPoccessedPlayerCharacter();
			if (PlayerCharacter)
			{
				//PlayerCharacter->Update1pMeshTransform(GetCameraLocation(), GetCameraRotation());
				//const float CurrentPitchValue = PlayerCharacter->CharacterMesh->GetRelativeTransform().Rotator().Pitch;
				//const float CameraPitch = Getre().Pitch;
				//float Delta = CameraPitch - CurrentPitchValue;
				//PlayerCharacter->CharacterMesh->AddRelativeRotation(FRotator(Delta, 0.f,0.f));
			}
		}
}
 