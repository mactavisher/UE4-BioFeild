// Fill out your copyright notice in the Description page of Project Settings.

#include "BFAttachment_Scope.h"
#include "Weapons/BFWeaponBase.h"
#include "Character/BFPlayerCharacter.h"
#include "Character/BFPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "BFComponents/BFWeaponMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"

ABFAttachment_Scope::ABFAttachment_Scope(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	RootComponent = MeshComp;
	SceneCaptureComp = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, TEXT("SceneCaptureComp"));
	SceneCaptureComp->RegisterComponent();
	SceneCaptureComp->SetComponentTickEnabled(true);
	AttachmentType = EAttachmentType::Scope;
}

void ABFAttachment_Scope::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RegisterAllComponents();
}

void ABFAttachment_Scope::BeginPlay()
{
	Super::BeginPlay();
	ShouldActiveScenceCapture();
   SceneCaptureComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ScenenCaptureAttachSocket);
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

void ABFAttachment_Scope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if WITH_EDITOR
	//DrawDebugSphere(GetWorld(), SceneCaptureComp->GetComponentLocation(), 15.f, 30.f, FColor::Red, false, 0.1f);
	//DrawDebugSphere(GetWorld(), MeshComp->GetSocketLocation("SightPoint"),15.f,30.f,FColor::Green,false,0.1f);
#endif
}

