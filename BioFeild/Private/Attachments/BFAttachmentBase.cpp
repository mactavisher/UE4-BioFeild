// Fill out your copyright notice in the Description page of Project Settings.

#include "BFAttachmentBase.h"
#include "components/SphereComponent.h"
#include "Weapons/BFWeaponBase.h"
#include "components/StaticMeshComponent.h"
#include "BFComponents/BFWeaponMeshComponent.h"

// Sets default values
ABFAttachmentBase::ABFAttachmentBase(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = ObjectInitailizer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("MeshComp"));
	InteractComp = ObjectInitailizer.CreateDefaultSubobject<USphereComponent>(this, TEXT("InteractComp"));
	RootComponent = MeshComp;
	InteractComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ABFAttachmentBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABFAttachmentBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

