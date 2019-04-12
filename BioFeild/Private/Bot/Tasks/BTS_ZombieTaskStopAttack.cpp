// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_ZombieTaskStopAttack.h"
#include "Bot/BFZombieController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTS_ZombieTaskStopAttack::UBTS_ZombieTaskStopAttack(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "Zombie Stop Attack";
}

EBTNodeResult::Type UBTS_ZombieTaskStopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABFZombieController* ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		ZombieController->StopAttack();
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTS_ZombieTaskStopAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTS_ZombieTaskStopAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{

}
