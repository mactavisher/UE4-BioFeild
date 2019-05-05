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


private:

		virtual void UpdateCamera(float DeltaTime)override;
	    
};
