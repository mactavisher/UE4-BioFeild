// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BFBaseCharacter.h"
#include "Character/BFPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Animation/BFAnimInstance.h"
#include "Components/TimelineComponent.h"
#include "BFPlayerCharacter.generated.h"

class ABFWeaponBase;
class UBFInventoryComponent;
class ABFPlayerController;
class UBFAnimInstance;

/** define a character's weapon Action Type */
namespace ECharacterWeaponAction
{
	enum Type
	{
		Idle,
		Equiping,
		UnEquiping,
		Firing,
	};
}

USTRUCT(BlueprintType)
struct FItemTraceDetectResult {

	GENERATED_USTRUCT_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 bHitSomething:1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AActor* HitActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8  bIsThreat:1;
};

/**
 *  Character for Player controlled Character
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUnequipedFinishedSignature); //takes no params to just let know weapon finish unequipped

UCLASS()
class BIOFEILD_API ABFPlayerCharacter : public ABFBaseCharacter
{
	GENERATED_UCLASS_BODY()

	/*create camera component for player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent*  CameraComp;

	/*create camera sprinArm  for Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent * CameraArmComp;

	/*create camera sprinArm  for Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UBFInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UTimelineComponent* AimingFOVTimeLineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCamera")
		float AimingCameraArmLenghModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TraceDetect")
		FItemTraceDetectResult DetectedItemInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Curve")
		UCurveFloat* AimingFOVCurve;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnWeaponUnequipedFinishedSignature OnUnequipWeapon;


protected:
	/** whether player is ADS or Aiming we can consider it as aiming */
	UPROPERTY()
	bool bIsAiming;

	/** specify whether this Character is armed */
	UPROPERTY()
	bool bIsArmed;

	/** indicate that character wants to swap weapon */
	UPROPERTY()
	bool bWantsToSwapWeapon;

	/** CurrentWeapon the player equipped */
	UPROPERTY()
	ABFWeaponBase* CurrentWeapon;

	/** weapon going to equipped by this character */
	UPROPERTY()
	ABFWeaponBase* WeaponToEquip;

	/** Timer Handle for equipping weapon*/
	UPROPERTY()
	FTimerHandle  EquipWeaponTimerHanle;

	/** Timer Handle for un_equipping weapon*/
	UPROPERTY()
	FTimerHandle  UnequipWeaponTimerHanle;

	/** Player controller controls this Character */
	UPROPERTY()
	ABFPlayerController* PlayerController;

	/** what is the character currently doing with weapons  */
	ECharacterWeaponAction::Type CharacterActionType;

	/** cached animation instance */
	UPROPERTY()
	UBFAnimInstance* CurrentAnimInstance;

	FOnTimelineFloat AimingFOVTimelineDelegate;

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	virtual void PostInitializeComponents()override;

	virtual void MoveRight(float Value);

	virtual void MoveForward(float Value);

	virtual void Jump()override;

	virtual void StopJumping()override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;

	virtual void EquipWeapon();

	virtual void FinishEquipWeapon();

	virtual void UnEquipWeapon();

	virtual void FinishUnEquipWeapon();

	virtual void AimingDispacher();

	virtual void StopAimingDispacher();

	virtual void ReloadWeapon();

	virtual void ToggleFireMode();

	virtual void DropCurrentWeapon();

	virtual void PickUpWeapon();

	virtual  bool CheckCanEquip();

	virtual void StartHoldBreath();

	virtual void StopHoldBreath();

	virtual void ToggleAimMode();

	virtual void NotifyItemDetected(AActor* DetectedItem);

	///////////////////////////////////////////
	/// equip Slot Weapons
	//////////////////////////////////////////

	/** specify to equip the slot1 weapon */
	virtual void EquipSlot1Weapon();

	/** specify to equip the slot2 weapon */
	virtual void EquipSlot2Weapon();

	/** specify to equip the slot3 weapon */
	virtual void EquipSlot3Weapon();

	/** specify to equip the slot4 weapon */
	virtual void EquipSlot4Weapon();

    /** if character is currently not equipped ,then searching for inventory and auto pick the first one reachable weapon  for character to equip */
	virtual void EquipDefault();

	/** Delegate bound function for unequipped weapon event  fired */
	UFUNCTION()
	virtual void OnWeaponUnequiped();



	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void Aiming();

	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void StopAiming();

	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void ADS();

	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void StopADS();

	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Action")
		void FireWeapon();

	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Action")
		void StopFireWeapon();


public:
	/** let Anim_BP knows that character is just armed,want to update animation  */
	UFUNCTION(BlueprintNativeEvent, Category = "BFCharacter")
		void OnCharacterArmed();

	/** let Anim_BP knows that character is just unarmed,want to update animation  */
	UFUNCTION(BlueprintNativeEvent, Category = "BFCharacter")
		void OnCharacterUnArmed();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFCharacter")
		virtual bool GetCharacterIsArmed()const { return bIsArmed; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFCharacter")
		virtual bool GetCharacterIsAming()const { return bIsAiming; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFCharacter")
		virtual ABFPlayerController* GetPlayerController()const { return PlayerController; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual void SetPlayerController(ABFPlayerController* NewController);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFCharacter")
		virtual bool GetCanEnterSprint();

	UFUNCTION()
		virtual void HandleStepOn(FHitResult Hit1, FHitResult Hit2);

	UFUNCTION(BlueprintImplementableEvent, Category = "BF|Character Action")
		void OnFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "BF|Character Action")
		void OnResetFire();

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		ABFWeaponBase* GetCurrentWeapon()const { return CurrentWeapon; }

	/** return camera location */
	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual FVector GetCameraLocation() { return CameraComp->GetComponentLocation(); }

	UFUNCTION(BlueprintCallable, Category = "Character|Weapons")
		virtual void AttatchWeaponToHand();

	UFUNCTION(BlueprintCallable, Category = "Character|Weapons")
		virtual void DetachCurrentWeaponFromHand();

	UFUNCTION()
		virtual void FreeStyleCamera();

	UFUNCTION()
		virtual void FixedStyleCamera();

	   virtual void DetectItem();

	   UFUNCTION()
		   virtual void AimingFOVDelegateCallBack();
	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		 virtual FItemTraceDetectResult GetTraceDetectResult()const { return DetectedItemInfo; }
};
