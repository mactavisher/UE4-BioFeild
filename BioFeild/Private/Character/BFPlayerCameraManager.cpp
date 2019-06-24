// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPlayerCameraManager.h"
#include "Character/BFPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Items/BFInventoryItem.h"
#include "Character/BFPlayerCharacter.h"

ABFPlayerCameraManager::ABFPlayerCameraManager(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	PrimaryActorTick.bCanEverTick = true;
	bEnableItemDetectTrace = true;
}

void ABFPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
	BFPlayerController = Cast<ABFPlayerController>(GetOwningPlayerController());
	if (BFPlayerController)
	{
		ABFPlayerCharacter* PlayerCharacter = BFPlayerController->GetPoccessedPlayerCharacter();
		if (PlayerCharacter)
		{
		}
	}
	if (bEnableItemDetectTrace)
	{
		FHitResult ItemTraceHitResult(ForceInit);
		GetItemDetectTraceHitResult(ItemTraceHitResult);
		if (ItemTraceHitResult.bBlockingHit)
		{
			if (ItemTraceHitResult.GetActor()->GetClass()->IsChildOf(ABFInventoryItem::StaticClass()))
			{
				//trace hit Inventory item type,and we want to notify the hit actor
				ABFInventoryItem* HitInventoryActor = Cast<ABFInventoryItem>(ItemTraceHitResult.GetActor());
				if (HitInventoryActor)
				{
					HitInventoryActor->ReceiveDetected(BFPlayerController, BFPlayerController->GetPoccessedPlayerCharacter(), BFPlayerController);
				}
			}
		}
	}
}

void ABFPlayerCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABFPlayerCameraManager::GetItemDetectTraceHitResult(FHitResult &OutTraceHitResult)
{
	FHitResult DetectTraceHitResult(ForceInit);
	const float TraceRange = 1000.f;
	const FVector CamLoc = GetCameraLocation();
	const FRotator CamRot = GetCameraRotation();
	const FVector TraceDir = TraceDir.GetSafeNormal(1.0f);//normalize vector
	const FVector TraceEnd = CamLoc + CamLoc * TraceDir*TraceRange;
	GetWorld()->LineTraceSingleByChannel(DetectTraceHitResult, CamLoc, TraceEnd, ECC_Camera);
	OutTraceHitResult = DetectTraceHitResult;
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), CamLoc, TraceEnd, FColor::Red, false, 0.5f);
#endif
}
