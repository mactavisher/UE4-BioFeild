// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attachments/BFAttachmentBase.h"
#include "BFAttachment_Scope.generated.h"

class USceneCaptureComponent2D;
/**
 * 
 */
UCLASS()
class BIOFEILD_API ABFAttachment_Scope : public ABFAttachmentBase
{
	GENERATED_UCLASS_BODY()
public:
		/** this scene Capture Component used to create dual scope effect */
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RenderTargetComp", meta = (AllowPrivateAccess = "true"))
		USceneCaptureComponent2D* SceneCaptureComp;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponAttachments")
			float CameraZOffSet;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SocketName")
			FName ScenenCaptureAttachSocket;
	
protected:
	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;
    
	virtual void ShouldActiveScenceCapture();

	virtual void Tick(float DeltaTime)override;

};
