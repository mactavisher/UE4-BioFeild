// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPlayerController_Menu.h"
#include "Character/BFPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/SpringArmComponent.h"


ABFPlayerController_Menu::ABFPlayerController_Menu(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
   
}

void ABFPlayerController_Menu::Possess(APawn* Pawn)
{
	LobbyCharacter = Cast<ABFPlayerCharacter>(Pawn);
	Super::Possess(LobbyCharacter);
	LobbyCharacter->DisableInput(this);
}

void ABFPlayerController_Menu::UnPossess()
{

}

void ABFPlayerController_Menu::BeginPlay()
{
	Super::BeginPlay();
	AudioComp = UGameplayStatics::SpawnSound2D(this, LobbyMusic);
	if (LobbyCharacter)
	{
		UCameraComponent* CharacterCamera = LobbyCharacter->CameraComp;
		//CharacterCamera->AttachToComponent(LobbyCharacter->GetBFSkeletalMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		const FVector CameraLocation = CharacterCamera->GetRelativeTransform().GetLocation();
		const FRotator CameraRotation = CharacterCamera->GetRelativeTransform().GetRotation().Rotator();
		FRotator NewCameraRotation(CameraRotation.Pitch+10.f, (CameraRotation.Yaw+180.f), CameraRotation.Roll);
		FVector NewCameraLocation(CameraLocation.X + 450.f, CameraLocation.Y, CameraLocation.Z-50);
		CharacterCamera->AddRelativeLocation(NewCameraLocation);
		CharacterCamera->AddRelativeRotation(NewCameraRotation);
	}
}

