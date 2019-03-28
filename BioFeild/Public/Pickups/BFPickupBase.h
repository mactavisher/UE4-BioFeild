// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFPickupBase.generated.h"

class UStaticMeshComponent;
class ABFBaseCharacter;
class USoundCue;
class USphereComponent;

UCLASS()
class BIOFEILD_API ABFPickupBase : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	/** interact components for player to interact with this pick up */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SphereComp", meta = (AllowPrivateAccess = "true"))
		USphereComponent* InteractComp;

	/** visual presentation of this pick up */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Display Name")
		FName DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp Sound")
		USoundCue* PickUpSound;

protected:
	/**check to see is this pickup still active*/
	bool bIsActive;

	/**check is this pick up is expired , will destroyed automatically when expired  */
	bool bIsExpired;

public:
	UFUNCTION(BlueprintCallable)
		virtual void SetIsActive(bool IsActive);

	UFUNCTION(BlueprintCallable)
		virtual void SetIsExpired(bool isExpired);

protected:
	/** play picked up sound effects */
	virtual void PlaySoundWhenPickedUp(USoundCue* PickUpSound);

	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
