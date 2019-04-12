// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "BFSkeletalMeshComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCanStepOnSignature, FHitResult, Hit1, FHitResult, Hit2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTakeProjectileDamageSignature, ABFProjectile*, HitProjectile, UBFSkeletalMeshComponent*, HitComponent);

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

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FTakeProjectileDamageSignature OnTakeProjectileDamage;

protected:

	UPROPERTY()
		FHitResult StepOnDetect1TraceHit;

	UPROPERTY()
		FHitResult StepOnDectct2TraceHit;

	UPROPERTY()
		bool bCanPerformStep;

	UPROPERTY()
		uint8 bTickDetectTrace : 1;

	UPROPERTY()
		ABFProjectile* HitProjectile;

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

	virtual void SetHitProjectile(ABFProjectile* HitProjectile);

	virtual void SetHitOwnerCharacter(ABFBaseCharacter* Owner);


public:
	/** return projectile that hit this skeletal mesh */
	UFUNCTION(BlueprintCallable, Category = "BFCharacter|Damage")
		virtual ABFProjectile* GetHitProjectile()const { return HitProjectile; }

	/** return character that own this mesh */
	UFUNCTION(BlueprintCallable, Category = "Character")
		virtual ABFBaseCharacter* GetCharacterOwner()const { return OwnerCharacter; }

	/** set this mesh owner */
	UFUNCTION(BlueprintCallable, Category = "Character")
		virtual void SetCharacterOwner(ABFBaseCharacter* Owner);

	/**calculate Pitch off set for aim off set value */
	UFUNCTION(BlueprintCallable, Category = "BFSkeletalMesh")
		virtual float CalculatePitchOffSet();

	/** receive projectile hit event */
	UFUNCTION()
		virtual void ReceiveProjectileHit(ABFProjectile* Projectile, float DamageAmount, FVector NormalImpulse);

	/** handle hit */
	UFUNCTION()
		virtual void HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	    virtual void BeginPlay()override;

public:
	virtual UBFAnimInstance* GetCurrentAnimInstance();

	virtual void DeactivateDetectTrace();
};
