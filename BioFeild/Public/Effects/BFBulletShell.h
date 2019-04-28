// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFBulletShell.generated.h"

class UParticleSystemComponent;
class USoundCue;
class ABFShellImpactEffects;
class ABFWeaponBase;

/**
 * shell to spawn when weapon shoots,this make weapon more realistic
 */
UCLASS(notplaceable)
class BIOFEILD_API ABFBulletShell : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	/** shell particle component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShellParticle", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* ShellParticleComp;

	/** shell particle template */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CollideSound")
		UParticleSystem* ShellParticle;

	/** shell collide effect class  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<ABFShellImpactEffects> ShellImpactEffectsClass;

protected:

	/** indicate if this effect is played ,to prevent from playing effects multiple times */
	UPROPERTY()
	    uint8 bIsEffectPlayed : 1;

	/** Weapon that eject this projectile */
	UPROPERTY()
	    ABFWeaponBase* WeaponEjector;

	virtual void BeginPlay()override;

	/** play shell collision sound when shell collide with other surfaces */
	UFUNCTION(BlueprintCallable, Category = "BulletShell")
		virtual void PlayShellCollisionSound(FName EventName, float  EmitterTime, int32  ParticleTime, FVector  Location, FVector  Velocity, FVector Direction, FVector  Normal, FName BoneName, UPhysicalMaterial* PhysMat);

public:
	/** weapon getter */
	UFUNCTION(BlueprintCallable, Category = "WeaponShell")
		virtual ABFWeaponBase* GetWeaponEjector()const { return WeaponEjector; }

	/** weapon setter */
	UFUNCTION(BlueprintCallable)
		virtual void SetWeaponEjector(ABFWeaponBase* InWeaponEjector) { this->WeaponEjector = InWeaponEjector; }
};
