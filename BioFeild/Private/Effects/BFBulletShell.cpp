// Fill out your copyright notice in the Description page of Project Settings.

#include "BFBulletShell.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Effects/BFImpactEffect.h"
#include "Engine/World.h"
#include "Weapons/BFWeaponBase.h"
#include "Sound/SoundCue.h"
#include "Effects/BFShellImpactEffects.h"

// Sets default values
ABFBulletShell::ABFBulletShell(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ShellParticleComp = ObjectInitailizer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ShellParticleComp"));
	RootComponent = ShellParticleComp;
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "PlayShellCollisionSound");
	ShellParticleComp->OnParticleCollide.AddUnique(Delegate);
	CurrentCollideCount = 0;
	InitialLifeSpan = 5.0f;
}

void ABFBulletShell::PlayShellCollisionSound(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat)
{
	if (CurrentCollideCount <= 0)
	{
		ABFShellImpactEffects* ShellImpactEffects = GetWorld()->SpawnActorDeferred<ABFShellImpactEffects>(ShellImpactEffectsClass, GetActorTransform());
		if (ShellImpactEffects)
		{
			ShellImpactEffects->SetPhysicalMat(PhysMat);
			ShellImpactEffects->SetCurrentCollideCount(CurrentCollideCount);
			UGameplayStatics::FinishSpawningActor(ShellImpactEffects, GetActorTransform());
		}
		CurrentCollideCount++;
	}
}

void ABFBulletShell::BeginPlay()
{
	Super::BeginPlay();
}

