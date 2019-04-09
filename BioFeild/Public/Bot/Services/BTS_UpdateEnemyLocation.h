// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_UpdateEnemyLocation.generated.h"

/**
 * 
 */
UCLASS()
class BIOFEILD_API UBTS_UpdateEnemyLocation : public UBTService_BlackboardBase
{
	GENERATED_UCLASS_BODY()
		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
};
