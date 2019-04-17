// Fill out your copyright notice in the Description page of Project Settings.

#include "BFShellImpactEffects.h"
#include "BioFeild.h"
#include "Materials/Material.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ABFShellImpactEffects::ABFShellImpactEffects(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ABFShellImpactEffects::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	USoundCue* ImpactSound = nullptr;
	ImpactSound = GetShellImpactSound();
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
}

USoundCue* ABFShellImpactEffects::GetShellImpactSound()
{
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysicalMat);
	USoundCue* SelectedSound = nullptr;
	switch (SurfaceType)
	{
	case SurfaceType_Default:SelectedSound = ShellImactSoundData.DefaultSound; break;
	case SurfaceType_Dirt:SelectedSound = ShellImactSoundData.DirtImpactSound; break;
	case SurfaceType_Concret:SelectedSound = ShellImactSoundData.DirtImpactSound; break;
	case SurfaceType_ThinMetal:SelectedSound = ShellImactSoundData.MetalImpactSound; break;
	case SurfaceType_ThickMetal:SelectedSound = ShellImactSoundData.MetalImpactSound; break;
	case SurfaceType_Grass:SelectedSound = ShellImactSoundData.DirtImpactSound; break;
	case SurfaceType_Glass:SelectedSound = ShellImactSoundData.GlassImpactSound; break;
	case SurfaceType_Water:SelectedSound = ShellImactSoundData.WaterImpactSound; break;
	case SurfaceType_Wood:SelectedSound = ShellImactSoundData.WoodImpactSound; break;
	case SurfaceType_Alsphat:SelectedSound = ShellImactSoundData.DefaultSound; break;
	}
	return SelectedSound;
}


