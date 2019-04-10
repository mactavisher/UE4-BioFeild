// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BFZombieController.generated.h"

class ABFZombie;
class ABFPlayerCharacter;
class ABFPlayerController;
class UBehaviorTreeComponent;
class ABFPickupBase;
class ABFPickup_Ammo;
class ABFPickup_Health;

/**
*   zombie controller is responsible for the behavior of zombies, it derived from AIController class
*/
UCLASS()
class BIOFEILD_API ABFZombieController : public AAIController
{
	GENERATED_UCLASS_BODY()

public:
	/** create behavior tree component for zombie */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Behaviour Tree")
		UBehaviorTreeComponent* BehaviorTreeComponent;

	/** ammo pick_up to  spawn class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		TSubclassOf<ABFPickup_Health> AmmoRewardClass;

	/** health pick_up to  spawn class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		TSubclassOf<ABFPickup_Ammo> HealthRewardClass;

	/** if zombie lost sight Threshold, and after this time player not seen again , means zombie will lost the player */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		float LostSightTime;

	/** within this radius to player can zombie perform a attack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		float AttackableRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		float FocusTime;

protected:
	/** zombie reference  that controlled by this controller */
	UPROPERTY()
		ABFZombie* PossessedZombie;

	/** player character reference that act as enemy for this zombie */
	UPROPERTY()
		ABFPlayerController* PlayerEnemy;

	/** is this controlled zombie dead or not */
	UPROPERTY()
		bool bisDead;

	UPROPERTY()
		bool bisFriend;

	/** lost target */
	UPROPERTY()
		bool bLostTarget;

	/** is zombie has a target to tracking? */
	UPROPERTY()
		bool bHasTarget;

	/** if hear any noise , whether or not to check the location */
	UPROPERTY()
		bool bWantsToTrackingNoise;

	/** if hear any noise , whether or not to check the location */
	UPROPERTY()
		bool bIsTrackingNoise;

	/** indicate that player is attackable  */
	UPROPERTY()
		bool bIsPlayerInAttackRange;

	/** whether or not to perform attack if player is attackable */
	UPROPERTY()
		bool bWantsToAttack;

	/** next place zombie want to go to */
	FVector NextMoveToLocation;

	/** the destination zombie wish to reach */
	FVector DesireDestination;

	/** is this zombie reached it's destination */
	bool bHasReachDestination;

	/** collection path locations to a specific location */
	TArray<FVector> PathPoints;

	/** timer to handle zombie lost player target */
	UPROPERTY()
		FTimerHandle LostPlayerTimerHanle;

	UPROPERTY()
		FTimerHandle ZombieAttackingHandle;

	UPROPERTY()
		FTimerHandle AddControllerInputHandle;

	virtual void Tick(float DeltaTime)override;

	virtual void BeginPlay()override;

	virtual void WalkTo();

	virtual void ChargeTo();

	virtual void CrawlerTo();

	virtual void AttackPlayer();

	virtual void StopMovement()override;

	/** call to decide whether give up this target or tracking it each time sees player  */
	virtual bool DecideToTrackingPlayer();

	/** call to decide whether give up this target or tracking it each time  hear something  Noise  and update key value on blackboard asset*/
	virtual void DecideToTrackingNoise(class ABFBaseCharacter* NoiseMaker,FVector NoiseLocation);

	void PostInitializeComponents()override;

	/** called when zombie lost the player as target */
	virtual void LostPlayer();

	UFUNCTION(BlueprintCallable)
		virtual FVector FindNextMoveToLocation();

	UFUNCTION(BlueprintCallable)
		virtual FVector FindeNextSearchLocation();


public:
	/** implement possess function*/
	virtual void Possess(APawn* InPawn)override;

	/** implement unpossess function*/
	virtual void UnPossess()override;

	/** make zombie to give up tracking player enemy */
	virtual void DiscardCurrentPlayer();

	/** set player as the enemy */
	UFUNCTION(BlueprintCallable, Category = "AI|Zombie")
		virtual void SetPlayerEnemy(ABFPlayerController* Player) { this->PlayerEnemy = Player; }

	/** return player enemy */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI|Zombie")
		virtual  ABFPlayerController* GetPlayerEnemy()const { return PlayerEnemy; }

	/** Get zombie possessed by this controller*/
	UFUNCTION(BlueprintCallable, Category = "AI|Zombie")
		virtual ABFZombie* GetPossessedZombie()const { return PossessedZombie; }

	/** if killed by player , randomly spawn pick up for player to pick*/
	UFUNCTION(BlueprintCallable, Category = "AI|Zombie")
		virtual void SpawnPickUps();

	/** return has zombie reached it's destination ?*/
	UFUNCTION(BlueprintCallable, Category = "AI|Zombie")
		virtual bool GetZombieHasReachDestination()const { return bHasReachDestination; }

	/** return zombie is dead or not */
	UFUNCTION(BlueprintCallable, Category = "AI|Zombie")
		virtual bool GetIsZombieDead()const { return PossessedZombie->GetCharacterIsDead(); }


	UFUNCTION()
		void OnZombieDead();

	/**return cached path points to a location*/
	UFUNCTION(BlueprintCallable, Category = "AI|Zombie")
		TArray<FVector> GetPathPoints()const { return PathPoints; }

	/** receive zombie hear noise and tell the controller to deal with it */
	UFUNCTION()
		virtual void ReceiveZombieHearNoise(ABFPlayerCharacter*  PlayerCharacter, const FVector& Location, float Volume);

	/** receive zombie see player and tell the controller to deal with it */
	UFUNCTION()
		virtual void ReceiveZombieSeePlayer(ABFPlayerController* PlayerController, ABFPlayerCharacter* PlayerPawn, FVector Location);

	/** find path points to a location*/
	UFUNCTION(BlueprintCallable, Category = "AI|Zombie")
		virtual void FindPathPointsToMove();
};
