// Fill out your copyright notice in the Description page of Project Settings.

#include "BFAttachment_Scope.h"
#include "Weapons/BFWeaponBase.h"
#include "Character/BFPlayerCharacter.h"
#include "Character/BFPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "BFComponents/BFWeaponMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/SceneCaptureComponent2D.h"

ABFAttachment_Scope::ABFAttachment_Scope(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	RootComponent = MeshComp;
	SceneCaptureComp = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, TEXT("SceneCaptureComp"));
	AttachmentType = EAttachmentType::Scope;
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

void ABFAttachment_Scope::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABFAttachment_Scope::BeginPlay()
{
	Super::BeginPlay();
    SceneCaptureComp->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ScenenCaptureAttachSocket);
	//EnableSceneCapture();
	ScopeGrassMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, ScopeGrassMat)->GetMaterial();
	ADSMat = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, ADSMat)->GetMaterial();
	SceneCaptureComp->CreatePhysicsState();
	MeshComp->SetMaterial(1, ADSMat); // apply this material one time ,and then switch to the default will prevent game stuck run in time when apply this render target texture
	MeshComp->SetMaterial(1, ScopeGrassMat);
	EnableSceneCapture();
	DisableSceneCapture();//turn off scencaptrue by default;
}


void ABFAttachment_Scope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFAttachment_Scope::DisableSceneCapture()
{
	SceneCaptureComp->bCaptureEveryFrame = false;
	SceneCaptureComp->Deactivate();
	SceneCaptureComp->bCaptureOnMovement = false;
	SceneCaptureComp->PrimaryComponentTick.SetTickFunctionEnable(false);
}

void ABFAttachment_Scope::EnableSceneCapture()
{
	SceneCaptureComp->bCaptureEveryFrame = true;
	SceneCaptureComp->Activate();
	SceneCaptureComp->bCaptureOnMovement = true;
	SceneCaptureComp->PrimaryComponentTick.SetTickFunctionEnable(true);
}

void ABFAttachment_Scope::OnADS()
{
	EnableSceneCapture();
	MeshComp->SetMaterial(1, ADSMat);
}

void ABFAttachment_Scope::OnStopADS()
{
	DisableSceneCapture();
	MeshComp->SetMaterial(1, ScopeGrassMat);
}

