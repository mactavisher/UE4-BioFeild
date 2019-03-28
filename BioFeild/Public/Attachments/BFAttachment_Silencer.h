// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attachments/BFAttachmentBase.h"
#include "BFAttachment_Silencer.generated.h"

class USoundCue;


/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFAttachment_Silencer : public ABFAttachmentBase
{

	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		USoundCue* FireSound;

public:
	virtual USoundCue* GetSilencerFireSound()const { return FireSound; }
};
