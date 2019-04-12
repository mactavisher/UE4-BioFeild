// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_ZombieStopMove.h"
#include "Bot/BFZombieController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
UBTT_ZombieStopMove::UBTT_ZombieStopMove(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "Stop Zombie Move";
}

EBTNodeResult::Type UBTT_ZombieStopMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	ABFZombieController* ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		ZombieController->StopMoveZombie();
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTT_ZombieStopMove::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTT_ZombieStopMove::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{

}
