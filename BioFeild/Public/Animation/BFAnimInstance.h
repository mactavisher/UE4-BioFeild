// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapons/BFWeaponBase.h"
#include "BFAnimInstance.generated.h"

class ABFPlayerCharacter;
class UBFSkeletalMeshComponent;
class UBFCharacterMovementComponent;
class ABFWeaponBase;

/**
 *
 */
UCLASS()
class BIOFEILD_API UBFAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bIsIdle : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bCanWalking : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bIsWalking : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bCanStopWalking : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bCanSprint : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bIsSprinting : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bCanCrouch : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bIsCrouching : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bIsArmed : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bIsFalling : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bCanProne : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bIsProne : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bIsInAir : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "State")
		uint8 bCanJump : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "state")
		uint8 bisADS:1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State")
		uint8 bisAiming:1;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "State")
		ABFPlayerCharacter* Character;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "State")
		float PitchOffset;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "State")
		float Direction;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "State")
		float SpeedXY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
		UBFSkeletalMeshComponent* CharacterSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
		UBFCharacterMovementComponent* CharacterMovementComp;

	UPROPERTY(VisibleAnywhere, Category = "State")
		EWeaponType CurrentWeaponType;

	/** native update for variables tick */
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	/** native initialize Animation implementation */
	virtual void NativeInitializeAnimation()override;

	/** calculate Direction between character forward vector and Input vector */
	virtual float  CalculateDirection();

	/** calculate horizontal speed */
	virtual float GetXYSpeed();

};
