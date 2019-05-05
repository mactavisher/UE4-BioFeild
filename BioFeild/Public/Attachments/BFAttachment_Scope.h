// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attachments/BFAttachmentBase.h"
#include "BFAttachment_Scope.generated.h"

class USceneCaptureComponent2D;
class UMaterialInterface;

/**
 *  base scope class for all scope based sights
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

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ScopeName")
			FName ScopeName;

		/** material class for scope grass when scene capture is disabled */
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ScopeMaterial")
	    UMaterialInterface* ScopeGrassMat;

		/** material class for scope grass when scene capture is disabled */
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ScopeMaterial")
			UMaterialInterface* ADSMat;

	
protected:
	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

public:
	virtual void DisableSceneCapture();

	virtual void EnableSceneCapture();

	virtual void OnADS();

	virtual void OnStopADS();
};
