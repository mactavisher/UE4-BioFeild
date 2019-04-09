// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BFZombieAnimation.generated.h"

class ABFZombie;
class ABFZombieController;

/**
 *   customized animation  instance for zombies
 */
UCLASS()
class BIOFEILD_API UBFZombieAnimation : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

public:
	/** indicate whether this zombie can walk or not*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bCanWalk : 1;

	/** indicate whether this zombie currently walking? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bIsWalking : 1;

	/** indicate whether this zombie can charge */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bCanCharge : 1;

	/** indicate whether this zombie currently Charging? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bIsCharging : 1;

	/** indicate whether this zombie currently Dead? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bIsDead : 1;

	/** indicate whether this zombie wants to patrol*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bWantToPatrol : 1;

	/** indicate whether this zombie currently patrolling? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bIsPatrolling : 1;
    
	/** indicate whether this zombie can attack? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bCanAttack : 1;

	/** indicate whether this zombie currently performs attacking ? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bIsAttacking : 1;

	/** indicate whether this zombie is alerted such as hearing some noise */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
		uint8 bIsAlerted : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zombie")
		ABFZombie* Zombie;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zombie")
		ABFZombieController* ZombieController;




protected:
	virtual void NativeInitializeAnimation()override;

	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

};
