// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFBulletShell.generated.h"

class UParticleSystemComponent;
class USoundCue;
class ABFShellImpactEffects;
class ABFWeaponBase;

UCLASS()
class BIOFEILD_API ABFBulletShell : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShellParticle", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* ShellParticleComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CollideSound")
		UParticleSystem* ShellParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<ABFShellImpactEffects> ShellImpactEffectsClass;

protected:

	int32 CurrentCollideCount;

	ABFWeaponBase* WeaponEjector;

	UFUNCTION(BlueprintCallable, Category = "BulletShell")
		virtual void PlayShellCollisionSound(FName EventName, float  EmitterTime, int32  ParticleTime, FVector  Location, FVector  Velocity, FVector Direction, FVector  Normal, FName BoneName, UPhysicalMaterial* PhysMat);

	virtual void BeginPlay()override;

public:
	UFUNCTION(BlueprintCallable, Category = "WeaponShell")
		virtual ABFWeaponBase* GetWeaponEjector()const { return WeaponEjector; }

	UFUNCTION(BlueprintCallable)
		virtual void SetWeaponEjector(ABFWeaponBase* WeaponEjector) { this->WeaponEjector = WeaponEjector; }
};
