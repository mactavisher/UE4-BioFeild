// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_ZombieUpdateHasReachNoiseLoc.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bot/BFZombieController.h"
#include "Character/BFPlayerCharacter.h"
#include "Bot/BFZombie.h"

UBTS_ZombieUpdateHasReachNoiseLoc::UBTS_ZombieUpdateHasReachNoiseLoc(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	Interval = 0.2f;
	RandomDeviation = 0.1;
	NodeName = "Update Zombie has reach Noise Location";
}


void UBTS_ZombieUpdateHasReachNoiseLoc::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ABFZombieController* ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	if (ZombieController)
	{
		const ABFZombie* Zombie = ZombieController->GetPossessedZombie();
		if (Zombie)
		{
			const FVector ZombieCurrentLoation = Zombie->GetActorLocation();
			const FName NoiseLocationKey = "NoiseLocation";
			const FVector NoiseLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(NoiseLocationKey);
			const FName HasReachNoiseLocationKey = GetSelectedBlackboardKey();
			const float Distance = FVector::Distance(NoiseLocation, ZombieCurrentLoation);
			const float DistanceLimit = FMath::FRandRange(20.f, 30.f);
			const bool bHasReachNoiseLocation = Distance > DistanceLimit ? true : false;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(HasReachNoiseLocationKey, bHasReachNoiseLocation);
		}
	}
}
