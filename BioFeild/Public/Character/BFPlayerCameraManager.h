// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BFPlayerCameraManager.generated.h"

class ABFPlayerController;

/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_UCLASS_BODY()

		UPROPERTY()
		ABFPlayerController* BFPlayerController;

	UPROPERTY(Transient)
		uint8 bEnableItemDetectTrace:1;


private:

		virtual void UpdateCamera(float DeltaTime)override;

		virtual void Tick(float DeltaSeconds)override;

		virtual void  GetItemDetectTraceHitResult(FHitResult &TraceHitResult);
	    
};
