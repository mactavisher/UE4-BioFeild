// Fill out your copyright notice in the Description page of Project Settings.

#include "BFProjectile.h"
#include "Effects/BFImpactEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Weapons/BFWeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "BFComponents/BFSkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/BFBaseCharacter.h"
#include "Bot/BFZombie.h"
#include "Character/BFPlayerCharacter.h"

ABFProjectile::ABFProjectile(const FObjectInitializer&ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;  // turn off tick to improve performance because we don't need it !
	ColisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision Comp"));
	RootComponent = ColisionComp;
	ProjectileMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh Comp"));
	ProjectileMoveComp = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement Comp"));
	TracerParticle = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Trace Particle"));
	ProjectileMoveComp->UpdatedComponent = ColisionComp;
	ProjectileMesh->SetupAttachment(ColisionComp);
	TracerParticle->SetupAttachment(ColisionComp);
	InitialLifeSpan = 10.f;
	ColisionComp->OnComponentHit.AddDynamic(this, &ABFProjectile::OnHit);
}

void ABFProjectile::SpawnImpactEffect(const FHitResult& Hit)
{
	const FTransform ImpactActorSpawnTransform(Hit.ImpactPoint.Rotation(), Hit.ImpactPoint, FVector::ZeroVector);
	if (ImapactEffectsClass)
	{
		ABFImpactEffect* ImpactActor = GetWorld()->SpawnActorDeferred<ABFImpactEffect>(ImapactEffectsClass, ImpactActorSpawnTransform);
		if (ImpactActor)
		{
			ImpactActor->HitResult = Hit;
			UGameplayStatics::FinishSpawningActor(ImpactActor, ImpactActorSpawnTransform);
		}
	}
}

void ABFProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ProjectileMoveComp->bInitialVelocityInLocalSpace = true;
	ProjectileMoveComp->bShouldBounce = false;
	/** turn this on because projectile's collision should be accurate to detect hit events and it's fast moving  */
	ColisionComp->SetAllUseCCD(true);
	ColisionComp->bReturnMaterialOnMove = true;
}

/** calculate damage should be applied to Character base on the bone hit by this projectile */
float ABFProjectile::DeterminDamage(const FHitResult& HitResult)
{
	float DamageAmount = WeaponOwner->GetWeaponData().BaseDamage;
	if (HitResult.BoneName.IsEqual("head"))
	{
		return DamageAmount = DamageAmount*FMath::FRandRange(3.5f, 4.0f);
	}
	if (HitResult.BoneName.IsEqual("neck_01"))
	{
		return DamageAmount = DamageAmount*FMath::FRandRange(2.5f, 3.f);
	}
	if (HitResult.BoneName.IsEqual("spine_01") || HitResult.BoneName.IsEqual("spine_02") || HitResult.BoneName.IsEqual("spine_03"))
	{
		return DamageAmount = DamageAmount*FMath::FRandRange(1.2f, 1.3f);
	}
	return DamageAmount*FMath::FRandRange(0.8f, 1.f);
}

void ABFProjectile::AddHitFeedBackToPlayerController(ABFBaseCharacter* HitCharacter)
{

}

void ABFProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp)
	{
		if (OtherComp->Mobility==EComponentMobility::Movable&&!OtherComp->GetClass()->IsChildOf(USkeletalMeshComponent::StaticClass()))
		{
			OtherComp->SetSimulatePhysics(true);
			OtherComp->AddImpulseAtLocation(GetVelocity()*0.5f, GetActorLocation(), NAME_None);
		}
		UBFSkeletalMeshComponent* HitSkeleton = Cast<UBFSkeletalMeshComponent>(OtherComp);
		if (HitSkeleton)
		{
			HitSkeleton->ReceiveProjectileHit(this, DeterminDamage(Hit), NormalImpulse,Hit);
		}
		SpawnImpactEffect(Hit);
	}
	Destroy(this);
}

float  ABFProjectile::GetDamageTaken()
{
	return DeterminDamage(HitResult);
}

