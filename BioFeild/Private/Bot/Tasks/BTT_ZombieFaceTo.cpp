// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_ZombieFaceTo.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Bot/BFZombieController.h"
#include "Bot/BFZombie.h"
#include "Character/BFPlayerController.h"
#include "Character/BFPlayerCharacter.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

UBTT_ZombieFaceTo::UBTT_ZombieFaceTo(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "Zombie Face To ";

}

EBTNodeResult::Type UBTT_ZombieFaceTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABFZombieController*const ZombieController = Cast<ABFZombieController>(OwnerComp.GetAIOwner());
	ABFZombie* Zombie = ZombieController->GetPossessedZombie();
	const FName Key = GetSelectedBlackboardKey();
	const FVector FaceToLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Key);
	if (Zombie)
	{
		FVector FocalPoint = Zombie->GetActorLocation();
		if (!FocalPoint.IsZero() &&Zombie)
		{
			FVector Direction = FaceToLocation - FocalPoint;
#if WITH_EDITOR
			DrawDebugSphere(GetWorld(), FocalPoint, 20.f, 30.f, FColor::Red, false, 0.1f);
			DrawDebugSphere(GetWorld(), FaceToLocation, 20.f, 30.f, FColor::Red, false, 0.1f);
			DrawDebugLine(GetWorld(), FocalPoint, Direction * 5000,FColor::Red,false,0.1f);
#endif
			FRotator NewControlRotation = Direction.Rotation();
			NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);
			ZombieController->SetControlRotation(NewControlRotation);
			Zombie->FaceRotation(NewControlRotation,GetWorld()->GetDeltaSeconds());
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTT_ZombieFaceTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTT_ZombieFaceTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{

}

