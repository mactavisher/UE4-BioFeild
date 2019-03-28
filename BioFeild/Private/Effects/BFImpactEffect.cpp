// Fill out your copyright notice in the Description page of Project Settings.

#include "BFImpactEffect.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include"Kismet/GameplayStatics.h"
#include"Particles/ParticleSystem.h"
#include"Sound/SoundCue.h"
#include"BioFeild.h"
#include"Materials/Material.h"
#include"Components/PrimitiveComponent.h"

// Sets default values
ABFImpactEffect::ABFImpactEffect(const FObjectInitializer&ObjectInitializer) :Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bAutoDestroyWhenFinished = true;
}
void ABFImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UPhysicalMaterial * PhysicalMat = HitResult.PhysMaterial.Get();
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysicalMat);
	UParticleSystem* ImpactParticles = GetImpactParticlesBySurfaceType(SurfaceType);
	USoundCue*ImpactSound = GetImpactSoundBySurfaceType(SurfaceType);
	UMaterial* ImpactDecal = GetDecalMaterialBySurfaceType(SurfaceType);
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticles, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
	}
	if (ImpactSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, HitResult.ImpactPoint);
	}
	if (ImpactDecal)
	{
		FRotator RandomDecalRotation = HitResult.ImpactNormal.Rotation();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
		UGameplayStatics::SpawnDecalAttached(ImpactDecal, FVector(5.f, 5.f, 5.f),
			HitResult.Component.Get(), HitResult.BoneName,
			HitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 10.f);
	}
	InitialLifeSpan = 2.0f;
}

/*Return the particlesSystem by SurfaceType we hit */
UParticleSystem* ABFImpactEffect::GetImpactParticlesBySurfaceType(EPhysicalSurface SurfaceType)
{
	UParticleSystem* ImpactParticle = nullptr;
	switch (SurfaceType)
	{
	case SurfaceType_Default:ImpactParticle = ImpactParticles.DefaultParticle; break;
	case SurfaceType_Dirt:ImpactParticle = ImpactParticles.DirtParticle; break;
	case SurfaceType_Concret:ImpactParticle = ImpactParticles.ConcretParticle; break;
	case SurfaceType_ThinMetal:ImpactParticle = ImpactParticles.ThinMetalParticle; break;
	case SurfaceType_ThickMetal:ImpactParticle = ImpactParticles.ThickMetalParticle; break;
	case SurfaceType_Grass:ImpactParticle = ImpactParticles.GrassParticle; break;
	case SurfaceType_Glass:ImpactParticle = ImpactParticles.GlassParticle; break;
	case SurfaceType_Water:ImpactParticle = ImpactParticles.WaterParticle; break;
	case SurfaceType_Wood:ImpactParticle = ImpactParticles.WoodParticle; break;
	case SurfaceType_Alsphat:ImpactParticle = ImpactParticles.AlsphatParticle; break;
	case SurfaceType_Ruber:ImpactParticle = ImpactParticles.RuberParticle; break;
	case SurfaceType_Body:ImpactParticle = ImpactParticles.BodyParticle; break;
	case SurfaceType_Gravel:ImpactParticle = ImpactParticles.GravelParticle; break;
	case SurfaceType_Mud:ImpactParticle = ImpactParticles.MudParticle; break;
	case SurfaceType_Can:ImpactParticle = ImpactParticles.CanParticle; break;
	case SurfaceType_Fabric: ImpactParticle = ImpactParticles.FabricParticle; break;
	case SurfaceType_Plastic: ImpactParticle = ImpactParticles.PlasticParticle; break;
	case SurfaceType_Carpet: ImpactParticle = ImpactParticles.CarpetParticle; break;
	case SurfaceTYpe_TreeLog: ImpactParticle = ImpactParticles.TreeLogParticle; break;
	case SurfaceType_TreeLeaves:ImpactParticle = ImpactParticles.TreeLeavesParticle; break;
	case SurfaceType_Armor:ImpactParticle = ImpactParticles.ArmorParticle; break;

	}
	return ImpactParticle;
}

/*return the SoundCue by SurfaceType We hit */
USoundCue* ABFImpactEffect::GetImpactSoundBySurfaceType(EPhysicalSurface SurfaceType)
{
	USoundCue* ImpactSound = nullptr;
	switch (SurfaceType)
	{
	case SurfaceType_Default:ImpactSound = ImpactSounds.DefaultSound; break;
	case SurfaceType_Dirt:ImpactSound = ImpactSounds.DirtSound; break;
	case SurfaceType_Concret:ImpactSound = ImpactSounds.ConcretSound; break;
	case SurfaceType_ThinMetal:ImpactSound = ImpactSounds.ThinMetalSound; break;
	case SurfaceType_ThickMetal:ImpactSound = ImpactSounds.ThickMetalSound; break;
	case SurfaceType_Grass:ImpactSound = ImpactSounds.GrassSound; break;
	case SurfaceType_Glass:ImpactSound = ImpactSounds.GlassSound; break;
	case SurfaceType_Water:ImpactSound = ImpactSounds.WaterSound; break;
	case SurfaceType_Wood:ImpactSound = ImpactSounds.WoodSound; break;
	case SurfaceType_Alsphat:ImpactSound = ImpactSounds.AlsphatSound; break;
	case SurfaceType_Ruber:ImpactSound = ImpactSounds.RuberSound; break;
	case SurfaceType_Body:ImpactSound = ImpactSounds.BodySound; break;
	case SurfaceType_Gravel:ImpactSound = ImpactSounds.GravelSound; break;
	case SurfaceType_Mud:ImpactSound = ImpactSounds.MudSound; break;
	case SurfaceType_Can:ImpactSound = ImpactSounds.CanSound; break;
	case SurfaceType_Fabric: ImpactSound = ImpactSounds.FabricSound; break;
	case SurfaceType_Plastic: ImpactSound = ImpactSounds.PlasticSound; break;
	case SurfaceType_Carpet: ImpactSound = ImpactSounds.CarpetSound; break;
	case SurfaceTYpe_TreeLog: ImpactSound = ImpactSounds.TreeLogSound; break;
	case SurfaceType_TreeLeaves:ImpactSound = ImpactSounds.TreeLeavesSound; break;
	case SurfaceType_Armor:ImpactSound = ImpactSounds.ArmorSound; break;
	}
	return ImpactSound;
}

/**
*  return decal material by surface type, notice that soft item  like water do not expect decals
*   but do assign it with nullptr otherwise it  will use the default decal
**/
UMaterial* ABFImpactEffect::GetDecalMaterialBySurfaceType(EPhysicalSurface surfaceType)
{
	UMaterial* ImpactDecal = nullptr;
	switch (surfaceType)
	{
	case SurfaceType_Default:ImpactDecal = ImpactDecals.DefaultDecal; break;
	case SurfaceType_Dirt:ImpactDecal = ImpactDecals.DirtDecal; break;
	case SurfaceType_Concret:ImpactDecal = ImpactDecals.ConcreteDecal; break;
	case SurfaceType_ThinMetal:ImpactDecal = ImpactDecals.MetalDecal; break;
	case SurfaceType_ThickMetal:ImpactDecal = ImpactDecals.MetalDecal; break;
	case SurfaceType_Grass:ImpactDecal = nullptr; break;
	case SurfaceType_Glass:ImpactDecal = ImpactDecals.GlassDecal; break;
	case SurfaceType_Water:ImpactDecal = nullptr; break;
	case SurfaceType_Wood:ImpactDecal = ImpactDecals.WoodDecal; break;
	case SurfaceType_Alsphat:ImpactDecal = ImpactDecals.AlsphatDecal; break;
	case SurfaceType_Ruber:ImpactDecal = ImpactDecals.RubberDecal; break;
	case SurfaceType_Body:ImpactDecal = ImpactDecals.BodyDecal; break;
	}
	return ImpactDecal;
}
