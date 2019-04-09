// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_ZombieCheckCheckSelfIsDead.h"
#include "Bot/BFZombieController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_ZombieCheckCheckSelfIsDead::UBTS_ZombieCheckCheckSelfIsDead(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "Check To See Is Self Dead";
	//Interval = 0.f;
	//RandomDeviation = 0.5f;
}

void UBTS_ZombieCheckCheckSelfIsDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABFZombieController*  ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		 FName KeyName = GetSelectedBlackboardKey();
	     bool IsZombieDead = ZombieController->GetIsZombieDead();
		 OwnerComp.GetBlackboardComponent()->SetValueAsBool(KeyName, IsZombieDead);
	}
}