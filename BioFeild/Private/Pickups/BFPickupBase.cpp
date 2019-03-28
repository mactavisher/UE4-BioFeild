// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPickupBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Character/BFBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

ABFPickupBase::ABFPickupBase(const FObjectInitializer& Objectinitializer) :Super(Objectinitializer)
{
	InteractComp = Objectinitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Interact"));
	InteractComp->SetSphereRadius(50.f);
	PickUpMesh = Objectinitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PickUp Mesh"));
	RootComponent = PickUpMesh;
	InteractComp->SetupAttachment(RootComponent);
	InteractComp->OnComponentBeginOverlap.AddDynamic(this, &ABFPickupBase::OnOverlapBegin);
	InitialLifeSpan = 10.f;    //destroy this pick up after 10 seconds if not picked up
}

void ABFPickupBase::SetIsActive(bool IsActive)
{
	this->bIsActive = IsActive;
}

void ABFPickupBase::SetIsExpired(bool isExpired)
{
	this->bIsExpired = isExpired;
}

void ABFPickupBase::PlaySoundWhenPickedUp(USoundCue* PickUpSound)
{
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
	}
}

void ABFPickupBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	bIsActive = true;
	bIsExpired = false;
}

void ABFPickupBase::BeginPlay()
{
	Super::BeginPlay();
	//turn on physics simulation when this pick up is spawned;
	PickUpMesh->SetSimulatePhysics(true);
}

void ABFPickupBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
