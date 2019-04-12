// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSTask_ZombieAttack.h"
#include "Bot/BFZombieController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTSTask_ZombieAttack::UBTSTask_ZombieAttack(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "Zombie Attack";
}


EBTNodeResult::Type UBTSTask_ZombieAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABFZombieController* ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		ZombieController->AttackPlayer();
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTSTask_ZombieAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTSTask_ZombieAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{

}
