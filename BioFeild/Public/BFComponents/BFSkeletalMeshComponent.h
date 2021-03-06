// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "BFSkeletalMeshComponent.generated.h"


class ABFProjectile;
class ABFBaseCharacter;
class UBFAnimInstance;

USTRUCT(BlueprintType)
struct FSocketNames {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketNames")
		FName RifleHolsterSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketNames")
		FName PistolHolsterSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketNames")
		FName ScopeHolderSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketNames")
		FName CurrentWeaponPistolSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketNames")
		FName CurrentWeaponRifleSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketNames")
		FName LeftStepSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketNames")
		FName RightStepSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketName")
		FName MouthSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketName")
		FName FootSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketName")
		FName IronSightSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketName")
		FName BillBoardSocket;
};

/**
* BF Customized SkeletalMesh Component,for Character use purposes
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCanStepOnSignature, FHitResult, Hit1, FHitResult, Hit2);
UCLASS(Blueprintable)
class BIOFEILD_API UBFSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise")
		float StepLoudnessBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise")
		float StepNoiseEffectiveRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketNames")
		FSocketNames SocketNames;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FCanStepOnSignature OnCanStep;

protected:

	UPROPERTY()
		FHitResult StepOnDetect1TraceHit;

	UPROPERTY()
		FHitResult StepOnDectct2TraceHit;

	UPROPERTY()
	uint8  bCanPerformStep : 1;

	UPROPERTY()
		uint8 bTickDetectTrace : 1;

	UPROPERTY()
		ABFProjectile* LastHitProjectile;

	UPROPERTY()
		ABFBaseCharacter* OwnerCharacter;

	/** cached  current Animation instance  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		UBFAnimInstance* CurrentAnimInstance;

	/** simulate left foot step */
	UFUNCTION(BlueprintCallable, Category = "BFCharacter|FootStep")
		virtual FHitResult OnLeftStep();

	/** simulate right  foot step */
	UFUNCTION(BlueprintCallable, Category = "BFCharacter|FootStep")
		virtual FHitResult OnRightStep();

	virtual void LineTraceFromPevils();

	virtual void StepOnDetectTrace1();

	virtual void StepOnDetectTrace2();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)override;

	virtual bool CalculateCanStepOn();

	UFUNCTION(BlueprintCallable,Category = "BFSkeletalMesh")
		virtual void SetLastHitProjectile(ABFProjectile* HitProjectile) { this->LastHitProjectile = HitProjectile; };

	virtual void SetHitOwnerCharacter(ABFBaseCharacter* Owner);


public:
	/** return projectile that hit this skeletal mesh */
	UFUNCTION(BlueprintCallable, Category = "BFCharacter|Damage")
		virtual ABFProjectile* GetLastHitProjectile()const { return LastHitProjectile; }

	/** return character that own this mesh */
	UFUNCTION(BlueprintCallable, Category = "Character")
		virtual ABFBaseCharacter* GetCharacterOwner()const { return OwnerCharacter; }

	/** set this mesh owner */
	UFUNCTION(BlueprintCallable, Category = "Character")
		virtual void SetCharacterOwner(ABFBaseCharacter* Owner);

	/**calculate Pitch off set for aim off set value */
	UFUNCTION(BlueprintCallable, Category = "BFSkeletalMesh")
		virtual float CalculatePitchOffSet();

	/** receive projectile hit event,return is this mesh owner id dead or not */
	UFUNCTION()
		virtual bool ReceiveProjectileHit(ABFProjectile* Projectile, float DamageAmount, FVector NormalImpulse, const FHitResult HitResult);

	    virtual void BeginPlay()override;

public:
	virtual UBFAnimInstance* GetCurrentAnimInstance();

	virtual void DeactivateDetectTrace();
};
