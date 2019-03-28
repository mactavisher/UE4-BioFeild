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
	DeterminSoundData();
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
	case SurfaceType_Default:SelectedSound = CurrentSelectSoundData.DefaultSound; break;
	case SurfaceType_Dirt:SelectedSound = CurrentSelectSoundData.DirtImpactSound; break;
	case SurfaceType_Concret:SelectedSound = CurrentSelectSoundData.DirtImpactSound; break;
	case SurfaceType_ThinMetal:SelectedSound = CurrentSelectSoundData.MetalImpactSound; break;
	case SurfaceType_ThickMetal:SelectedSound = CurrentSelectSoundData.MetalImpactSound; break;
	case SurfaceType_Grass:SelectedSound = CurrentSelectSoundData.DirtImpactSound; break;
	case SurfaceType_Glass:SelectedSound = CurrentSelectSoundData.GlassImpactSound; break;
	case SurfaceType_Water:SelectedSound = CurrentSelectSoundData.WaterImpactSound; break;
	case SurfaceType_Wood:SelectedSound = CurrentSelectSoundData.WoodImpactSound; break;
	case SurfaceType_Alsphat:SelectedSound = CurrentSelectSoundData.DefaultSound; break;
	}
	return SelectedSound;
}

void ABFShellImpactEffects::DeterminSoundData()
{
	if (ShellImactSoundDatas.Num() <= 1)
	{
		CurrentSelectSoundData = ShellImactSoundDatas[0];
	}
	else {
		switch (CurrentCollideCount)
		{
		case 0:CurrentSelectSoundData = ShellImactSoundDatas[0]; break;
		case 1:CurrentSelectSoundData = ShellImactSoundDatas[1]; break;
		case 2: CurrentSelectSoundData = ShellImactSoundDatas[2]; break;
		}
	}
}

