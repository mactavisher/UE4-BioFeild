// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_ZombieUpdateEnemyLocChange.h"
#include "Bot/BFZombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bot/BFZombieController.h"
#include "Bot/BFZombieController.h"
#include "Character/BFPlayerController.h"
#include "Character/BFPlayerCharacter.h"
#include "Engine/Engine.h"

UBTS_ZombieUpdateEnemyLocChange::UBTS_ZombieUpdateEnemyLocChange(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "UpdateIsEnemyLocationChanged";
	Interval = 0.2f;
	RandomDeviation = 0.1f;
}

void UBTS_ZombieUpdateEnemyLocChange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const ABFZombieController* const ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		const ABFZombie* const Zombie = ZombieController->GetPossessedZombie();
		const ABFPlayerController* const PlayerController = ZombieController->GetPlayerEnemy();
		if (Zombie&&PlayerController)
		{
			const FName EnemyLocationKey = TEXT("EnemyLocation");
			const FName EnemyLocationHasChangedkey = GetSelectedBlackboardKey();
			const FVector LastEnemyLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(EnemyLocationKey);
			const FVector CurrentEnemyLocation = PlayerController->GetPoccessedPlayerCharacter()->GetActorLocation();
			const float AcceptableDistance = FMath::RandRange(20.f, 50.f);
			const float Distance = FMath::Abs(FVector::Distance(CurrentEnemyLocation, LastEnemyLocation));
			bool EnemyLocationChanged = Distance <= AcceptableDistance ? true : false;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(EnemyLocationHasChangedkey, EnemyLocationChanged);
		}
	}
}
