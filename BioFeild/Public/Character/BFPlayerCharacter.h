// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BFBaseCharacter.h"
#include "Character/BFPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Items/BFInventoryItem.h"
#include "Animation/BFAnimInstance.h"
#include "Components/TimelineComponent.h"
#include "BFPlayerCharacter.generated.h"

class ABFWeaponBase;
class UBFInventoryComponent;
class ABFPlayerController;
class UBFAnimInstance;
class UPawnNoiseEmitterComponent;

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

UENUM(BlueprintType)
enum class EViewMode :uint8
{
	TPS,
	FPS
};

USTRUCT(BlueprintType)
struct FItemTraceDetectResult {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 bHitSomething : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 bHitInventoryItem : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AActor* HitActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABFInventoryItem* HitItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8  bIsThreat : 1;

	FItemTraceDetectResult()
	{
		bHitSomething = false;
		bHitInventoryItem = false;
		HitActor = nullptr;
		HitItem = nullptr;
		bIsThreat = false;
	}
};

USTRUCT(BlueprintType)
struct FTargetHitInfo {

	GENERATED_USTRUCT_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float DamgeCause;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 bIsTargetDead : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AActor* Victim;

	FTargetHitInfo()
	{
		DamgeCause = 0.f;
		bIsTargetDead = false;
		Victim = nullptr;
	}
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpringArm", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArmComp;

	/*create camera sprinArm  for Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UBFInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UTimelineComponent* AimingFOVTimeLineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UPawnNoiseEmitterComponent* NoiseEmmiterComp;

	/*create 3rd view mesh for player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMesh", meta = (AllowPrivateAccess = "true"))
		UBFSkeletalMeshComponent*  Mesh3PComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TraceDetect")
		FItemTraceDetectResult DetectedItemInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Curve")
		UCurveFloat* AimingFOVCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ViewMode")
		EViewMode ViewMode;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnWeaponUnequipedFinishedSignature OnUnequipWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UBFCharacterWidget> CharacterWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UBFCurrentWeaponWidget> CurrenetWeaponWidgetClass;


protected:
	/** whether player is ADS or Aiming we can consider it as aiming */
	UPROPERTY()
		uint8 bIsADS : 1;

	UPROPERTY()
		FTargetHitInfo TargetHitInfo;

	/** specify whether this Character is armed */
	UPROPERTY()
		uint8 bIsArmed : 1;

	/** indicate that character wants to swap weapon */
	UPROPERTY()
		uint8 bWantsToSwapWeapon : 1;

	/** CurrentWeapon the player equipped */
	UPROPERTY()
		ABFWeaponBase* CurrentWeapon;

	/** weapon going to equipped by this character */
	UPROPERTY()
		ABFWeaponBase* WeaponToEquip;

	/** Timer Handle for equipping weapon*/
	UPROPERTY()
		FTimerHandle  EquipWeaponTimerHanle;

	/** Timer Handle for equipping weapon*/
	UPROPERTY()
		FTimerHandle  ResetTargetHitInfoTimerHandle;


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

	UPROPERTY()
		FTransform CameraOriginalRelativeTransform;

	UPROPERTY()
		UBFCharacterWidget* CharacterWidgetInstance;

	UPROPERTY()
		UBFCurrentWeaponWidget* CurrentWeaponWidgetInstance;

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

	virtual void CreateCharacterWidgetInstance();
   
	virtual void Sprint()override;

	virtual void StopSprint()override;

	virtual void NotifyItemDetected(ABFInventoryItem* DetectedItem);

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


	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Action")
		void FireWeapon();

	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Action")
		void StopFireWeapon();


public:
	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void ADS();

	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void StopADS();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFCharacter")
		virtual bool GetCharacterIsArmed()const { return bIsArmed; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFCharacter")
		virtual bool GetCharacterIsADS()const { return bIsADS; }

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

	virtual void InitializeUserWidget();

	UFUNCTION(BlueprintCallable)
	virtual FTargetHitInfo GetTargetHitInfo()const { return TargetHitInfo; }

	virtual void Update1pMeshTransform(const FVector& CameraLocation, const FRotator& CameraRotation);

	UFUNCTION()
	virtual void AimingFOVDelegateCallBack();

	UFUNCTION()
	virtual void ResetTargetHitInfo();

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
	virtual FItemTraceDetectResult GetTraceDetectResult()const { return DetectedItemInfo; }

	virtual UBFSkeletalMeshComponent* GetMesh3p()const { return Mesh3PComp; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
	virtual EViewMode GetViewMode()const { return ViewMode; }

	virtual FTransform GetCameraOriginalTransform()const { return CameraOriginalRelativeTransform; }

	virtual void ReceiveHitTarget(float DamageAmount, bool IsTargetDead, class ABFBaseCharacter* Victim);

	/** stop player current playing montage */
	virtual void  StopCurrentPlayingMontage();

	/** equip specific weapon by weapon slot index,if value equals 255 which is uint8's max value,means we want to equip default weapon */
	virtual void EquipSlotWeapon(uint8 WeaponSlotIndex);
};
