// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_ZombieCanSeeEnemy.h"
#include "Bot/BFZombieController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/BFPlayerController.h"
#include "Character/BFPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bot/BFZombie.h"

UBTS_ZombieCanSeeEnemy::UBTS_ZombieCanSeeEnemy(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	Interval = 0.1f;
	RandomDeviation = 0.1f;
	NodeName = "Update Can See Enemy";
}

void UBTS_ZombieCanSeeEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const ABFZombieController* const ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		const  ABFPlayerController* const Target = ZombieController->GetPlayerEnemy();
		const FName Key = GetSelectedBlackboardKey();
		bool HasTarget = false;
		if (Target)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(Key, true);

		}
		else {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(Key, false);
		}
	}
}
