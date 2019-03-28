// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "BFWeaponMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API UBFWeaponMeshComponent : public USkeletalMeshComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Sockets")
		FName MuzzleFlashSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Sockets")
		FName ShellEjectSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Sockets")
		FName ScopeSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Sockets")
		FName CameraSocket ;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Sockets")
		FName ScenceCaptrueSocket;
};
