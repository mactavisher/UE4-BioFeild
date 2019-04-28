// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/BFInventoryItem.h"
#include "BFAttachmentBase.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ABFWeaponBase;
class UBFWeaponMeshComponent;


UENUM(BlueprintType)
enum class EAttachmentType :uint8
{
	Silencer,
	Scope,
	Grip,
	Stock,
	FlashHider,
	LaserSight,
	FlashLight,
};

UCLASS()
class BIOFEILD_API ABFAttachmentBase : public ABFInventoryItem
{
	GENERATED_UCLASS_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		USphereComponent* InteractComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Type")
		EAttachmentType AttachmentType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/** weapon which owns this Scope */
	UPROPERTY()
		ABFWeaponBase* WeaponOwner;

public:
	virtual void SetWeaponOwner(ABFWeaponBase* NewWeaponOwner) { this->WeaponOwner = NewWeaponOwner; }

	virtual ABFWeaponBase*  GetWeaponOwner()const { return this->WeaponOwner; }
};
