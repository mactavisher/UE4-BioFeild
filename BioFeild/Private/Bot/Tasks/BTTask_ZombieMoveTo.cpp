// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ZombieMoveTo.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bot/BFZombieController.h"
#include "Bot/BFZombieController.h"
#include "Character/BFPlayerController.h"
#include "Character/BFPlayerCharacter.h"
#include "Engine/Engine.h"

UBTTask_ZombieMoveTo::UBTTask_ZombieMoveTo(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "ZombieMoveTo";
}

EBTNodeResult::Type UBTTask_ZombieMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABFZombieController*const ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTTask_ZombieMoveTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_ZombieMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{

}
