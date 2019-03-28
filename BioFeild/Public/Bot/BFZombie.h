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


//event signature syntaxes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeePlayerSignatrue, class ABFPlayerController*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHearPlayerSignature, ABFPlayerCharacter*, Instigator, const FVector&, Location, float, Volume);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCausePlayerDamageSignature, ABFZombie*, DamageCauser, float, DamageAmount, FHitResult, BodyHit);

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

	/** bill board component used for show zombie's health bar */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BillboardComp")
		UBillboardComponent* BillBoardComp;

	/** behavior tree assets used for this zombie  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour Tree Asset")
		UBehaviorTree* ZombieBehaviour;

	/** zombies attack animation ,can hold animation notify data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie|Animation")
		UAnimMontage* AttackAnim;

	/** attached to left hand to detect damage  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zombie|Damage", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* LeftHandDamage;

	/** attached to right hand to detect damage  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zombie|Damage", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RightHandDamage;

	/** base damage each time damage event happens  */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zombie|Damage", meta = (AllowPrivateAccess = "true"))
		float HandDamageBase;

	/** left hand damage detect comp attaching point */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie|Sockets")
		FName LeftHandDamageSocket;

	/** left hand damage detect comp attaching point */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zombie|Sockets")
		FName RightHandDamageSocket;

	/** event syntax to broadcast this damage player event */
	UPROPERTY(BlueprintAssignable, Category = "Zombie|Events")
		FOnCausePlayerDamageSignature OnDamagePlayer;

	/** event syntax to broadcast see player event */
	UPROPERTY(BlueprintAssignable, Category = "Zombie|Events")
		FOnSeePlayerSignatrue OnSeePlayer;

	/** event syntax to broadcast see Hear noise  event */
	UPROPERTY(BlueprintAssignable, Category = "Zombie|Events")
		FOnHearPlayerSignature OnHearPlayer;


protected:
	/** zombie controller reference  that possess this zombie  */
	UPROPERTY()
		ABFZombieController* ZombieController;

	/** current zombie state */
	UPROPERTY()
		EZombieState ZombieState;

	/** declare zombie attack event */
	UPROPERTY()
		FDamageEvent ZombieAttackDamageEvent;

	/** is zombie currently performs attacking */
	UPROPERTY()
		bool bIsAttacking;

	virtual void BeginPlay()override;

	virtual void PostInitializeComponents()override;

	/** enable left hand damage */
	virtual void EnableLeftHandDamage();

	/** enable right hand damage */
	virtual void EnableRighthandDamage();

	/** disable left hand damage */
	virtual void DisableLeftHandDamage();

	/** disable right hand damage */
	virtual void DisableRighthandDamage();

	/** decide damage amount base on collision info,currently ,will just simply return damage base, other detail cause effect damage will add on future */
	virtual float  DecideHandsDamage(const FHitResult & SweepResult, ABFPlayerCharacter* PlayerCharacter, bool bFromSweep);

public:

	/** override parent function to implement more logic  */
	virtual float PlayAttackingAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);



	UFUNCTION()
		virtual void OnSeePawn(APawn* Pawn);

	UFUNCTION()
		virtual void OnHearNoise(APawn* EnemyPawn, const FVector& Location, float Volume);

	/** deal with left hand damage */
	UFUNCTION()
		virtual void HandleLeftHandDamageOverlap(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** deal with right hand damage */
	UFUNCTION()
		virtual void HandleRightHandDamageOverlap(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		virtual void HandleLeftHandDamageEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void HandleRightHandDamageEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

	virtual void HandleDeath()override;
};
