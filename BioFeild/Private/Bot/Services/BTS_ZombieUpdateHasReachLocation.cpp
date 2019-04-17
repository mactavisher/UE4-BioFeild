// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_ZombieUpdateHasReachLocation.h"
#include "Bot/BFZombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BFPlayerCharacter.h"
#include "Engine/Engine.h"
UBTS_ZombieUpdateHasReachLocation::UBTS_ZombieUpdateHasReachLocation(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "UpdateHasReachEnemyLocation";
	Interval = 0.2f;
	RandomDeviation = 0.1f;
}

void UBTS_ZombieUpdateHasReachLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const ABFZombieController* const ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		const ABFZombie* const Zombie = ZombieController->GetPossessedZombie();
		if (Zombie)
		{
			const FName EnemyLocationKey = TEXT("EnemyLocation");
			const FName HasReachEnemyLocationKey = GetSelectedBlackboardKey();
			const FVector ZombieLocation = Zombie->GetActorLocation();
			const FVector EnemyLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(EnemyLocationKey);
			const float DistanceToEnemyLocation = FMath::Abs(FVector::Distance(EnemyLocation, ZombieLocation));
			const float DistanceLimit = FMath::RandRange(100.f, 102.f);
			bool HasReachEnemyLocation = DistanceToEnemyLocation <= DistanceLimit ? true : false;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(HasReachEnemyLocationKey, HasReachEnemyLocation);
			UE_LOG(LogTemp, Warning, TEXT("Zombie to enemy location is %f"), DistanceToEnemyLocation);
		}
	}
}
