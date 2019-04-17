// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFShellImpactEffects.generated.h"

class USoundCue;
USTRUCT(BlueprintType)
struct FShellImpactSoundData {

	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
		USoundCue* DefaultSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
		USoundCue* DirtImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
		USoundCue* ConcretImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
		USoundCue* MetalImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
		USoundCue* WaterImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
		USoundCue* GlassImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
		USoundCue* WoodImpactSound;
};

USTRUCT(BlueprintType)
struct FParticleImpactData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Particles")
		UParticleSystem* WaterImpactParticle;
};
UCLASS()
class BIOFEILD_API ABFShellImpactEffects : public AActor
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		FShellImpactSoundData ShellImactSoundData;

protected:

	UPhysicalMaterial* PhysicalMat;

public:


	virtual UPhysicalMaterial* GetPhysicalMat() { return PhysicalMat; }

	virtual void SetPhysicalMat(UPhysicalMaterial* PhysicalMat) { this->PhysicalMat = PhysicalMat; }

	virtual FShellImpactSoundData GetSoundImpactDatas()const { return ShellImactSoundData; }


	virtual void PostInitializeComponents()override;

	virtual USoundCue* GetShellImpactSound();
};
