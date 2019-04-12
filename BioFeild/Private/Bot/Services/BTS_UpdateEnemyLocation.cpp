// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_UpdateEnemyLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bot/BFZombieController.h"
#include "Character/BFPlayerCharacter.h"
#include "Bot/BFZombie.h"
UBTS_UpdateEnemyLocation::UBTS_UpdateEnemyLocation(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	Interval = 0.5f;
	RandomDeviation = 0.5f;
	NodeName = "Update Enemy Location";
}


void UBTS_UpdateEnemyLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ABFZombieController* ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		ABFPlayerController* Enemy = ZombieController->GetPlayerEnemy();
		if (Enemy)
		{
			FVector Location = Enemy->GetPoccessedPlayerCharacter()->GetActorLocation();
			ZombieController->SetEnemyLocation(Location);
			const FName Key = GetSelectedBlackboardKey();
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(Key, Location);
		}
	}
}
