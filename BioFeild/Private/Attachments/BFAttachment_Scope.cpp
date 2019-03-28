// Fill out your copyright notice in the Description page of Project Settings.

#include "BFAttachment_Scope.h"
#include "Weapons/BFWeaponBase.h"
#include "Character/BFPlayerCharacter.h"
#include "Character/BFPlayerController.h"
#include "BFComponents/BFWeaponMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"

ABFAttachment_Scope::ABFAttachment_Scope(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	SceneCaptureComp = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, TEXT("SceneCaptureComp"));
	SceneCaptureComp->RegisterComponent();
	SceneCaptureComp->SetComponentTickEnabled(true);
	AttachmentType = EAttachmentType::Scope;
}

void ABFAttachment_Scope::BeginPlay()
{
	Super::BeginPlay();
	SceneCaptureComp->AttachToComponent(GetWeaponOwner()->WeaponMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetWeaponOwner()->WeaponMeshComponent->ScenceCaptrueSocket);
	ShouldActiveScenceCapture();
}

void ABFAttachment_Scope::ShouldActiveScenceCapture()
{
	if (GetWeaponOwner())
	{
		if (GetWeaponOwner()->GetWeaponOwner())
		{
			if (GetWeaponOwner()->GetWeaponOwner()->GetController()->IsLocalController())
			{
				SceneCaptureComp->Deactivate();
			}
		}
		else
		{
			SceneCaptureComp->Activate();
		}
	}
}

