// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attachments/BFAttachment_Scope.h"
#include "BFAttachment_Magnifier.generated.h"

/**
 *   type of scope,used for creating blueprint class
 */
UCLASS()
class BIOFEILD_API ABFAttachment_Magnifier : public ABFAttachment_Scope
{
	GENERATED_UCLASS_BODY()
		
protected:

	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;
};
