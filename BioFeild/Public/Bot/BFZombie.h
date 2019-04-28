// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BFBaseCharacter.h"
#include "BFZombie.generated.h"


class UAnimMontage;
class UBehaviorTreeComponent;
class UPawnSensingComponent;
class UBehaviorTree;
class ABFPlayerCharacter;
class ABFPlayerController;
class UBoxComponent;
class UBillboardComponent;
class ABFZombieController;


/** struct to hold animation to react hit from different direction  */
USTRUCT(BlueprintType)
struct FHitAnim {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* FrontHitAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* LeftHitAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* RightHitAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* BackHitAnim;
};

/** enum to specify zombies state */
UENUM(BlueprintType)
enum class EZombieState :uint8 {
	Idle,
	Walking,
	Crawing,
	Sitting,
	Sleeping,
	Charging,
	Dead,
};

/** enum to specify zombies state */
UENUM(BlueprintType)
enum class EZombieMoveType :uint8 {
	
	WalkTo,
	ChargeTo,
	CrawlerTo,
};

//event signature syntaxes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSeePlayerSignatrue, class ABFPlayerController*, Player,class ABFPlayerCharacter*, PlayerPawn,FVector,Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHearPlayerSignature, ABFPlayerCharacter*, Instigator, const FVector&, Location, float, Volume);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOZomibeDeadSignature);

/**
*   zombie type of bot
*/
UCLASS()
class BIOFEILD_API ABFZombie : public ABFBaseCharacter
{
	GENERATED_UCLASS_BODY()

public:
	/** create sensing component for zombie */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zombie/SensingComp", meta = (AllowPrivateAccess = "true"))
		UPawnSensingComponent* ZombieSensingComp;

	/** behavior tree assets used for this zombie  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour Tree Asset")
		UBehaviorTree* ZombieBehaviour;

	/** zombies attack animation ,can hold animation notify data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie|Animation")
		UAnimMontage* AttackAnim;

	/** base damage each time damage event happens  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zombie|Damage", meta = (AllowPrivateAccess = "true"))
		float HandDamageBase;

	/** left hand damage detect comp attaching point */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie|Sockets")
		FName LeftHandDamageSocket;

	/** left hand damage detect comp attaching point */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie|Sockets")
		FName RightHandDamageSocket;

	/**notify zombie controller is see player */
	UPROPERTY(BlueprintAssignable, Category = "Zombie|Events")
		FOnSeePlayerSignatrue OnSeePlayer;

	/**notify zombie controller if hear player  */
	UPROPERTY(BlueprintAssignable, Category = "Zombie|Events")
		FOnHearPlayerSignature OnHearPlayer;

	/**notify zombie controller if dead  */
	UPROPERTY(BlueprintAssignable, Category = "Zombie|Events")
		FOZomibeDeadSignature OnZombieDead;


protected:
	/** zombie controller reference  that possess this zombie  */
	UPROPERTY()
		ABFZombieController* ZombieController;

	/** current zombie state */
	UPROPERTY()
		EZombieState ZombieState;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Zombie|Movement")
		EZombieMoveType MoveType;

	/** declare zombie attack event */
	UPROPERTY()
		FDamageEvent ZombieAttackDamageEvent;

	/** is zombie currently performs attacking */
	UPROPERTY()
		bool bIsAttacking;

	virtual void BeginPlay()override;

	virtual void PostInitializeComponents()override;

public:

	/** override parent function to implement more logic  */
	virtual float PlayAttackingAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	UFUNCTION()
		virtual void OnSeePawn(APawn* Pawn);

	UFUNCTION()
		virtual void OnHearNoise(APawn* EnemyPawn, const FVector& Location, float Volume);

	UFUNCTION(BlueprintCallable, Category = "Zombie")
		virtual bool GetIsZombieAttacking()const { return bIsAttacking; }

	UFUNCTION(BlueprintCallable, Category = "Zombie")
		virtual void SetIsZombieAttacking(bool Attacking) { bIsAttacking = Attacking; }

	UFUNCTION(BlueprintCallable, Category = "Zombie")
		ABFZombieController* GetZombieController()const { return ZombieController; }

	UFUNCTION(BlueprintCallable, Category = "Zombie")
		virtual void SetZombieController(ABFZombieController* NewZombieController) { this->ZombieController = NewZombieController; }

	UFUNCTION(BlueprintCallable, Category = "Zombie")
		virtual void SetZombieCurrentState(EZombieState NewState) { this->ZombieState = NewState; }

	UFUNCTION(BlueprintCallable, Category = "Zombie")
		virtual EZombieState GetZombieCurrentState()const { return ZombieState; }

	 


	UFUNCTION(BlueprintCallable, Category = "Zombie")
		virtual EZombieMoveType GetZombieMoveType()const { return MoveType; }

	virtual void HandleDeath()override;
};
