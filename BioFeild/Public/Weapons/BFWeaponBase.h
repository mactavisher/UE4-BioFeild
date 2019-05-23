// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BioFeild.h"
#include "Items/BFInventoryItem.h"
#include "Components/TimelineComponent.h"
#include "Engine/Canvas.h"
#include "BFWeaponBase.generated.h"

class UAnimMontage;
class UAnimationAsset;
class ABFPlayerCharacter;
class UTimelineComponent;
class ABFPlayerController;
class UBFWeaponMeshComponent;
class ABFProjectile;
class USoundCue;
class UParticleSystem;
class UCurveFloat;
class AActor;
class UBoxComponent;
class UStaticMesh;
class UTexture2D;
class FIconImage;
class ABFEmptyMagazine;
class ABFBulletShell;
class UUserWidget;
class ABFAttachment_Scope;
class ABFAttachment_Silencer;

/**weapon types can be assigned via blueprints*/
UENUM(BlueprintType)
enum class EWeaponType :uint8 {
	AssaultRifle                                             UMETA(DisplayName = "AssultRifle"),
	HandGun                                                UMETA(DisplayName = "HandGun"),
	ShotGun                                                 UMETA(DisplayName = "ShotGun"),
	Sniper                                                    UMETA(DisplayName = "Sniper")
};

UENUM(BlueprintType)
enum class EWeaponNames :uint8 {
	AK47                                                   UMETA(DisplayName = "AK47"),
	SKS                                                     UMETA(DisplayName = "SKS"),
	ShotGun                                              UMETA(DisplayName = "ShotGun"),
	SCAR                                                  UMETA(DisplayName = "SCAR"),
	XM8                                                    UMETA(DisplayName = "XM8"),
	CLOCK18                                             UMETA(DisplayName = "Clock18"),
	MP7                                                    UMETA(DisplayName = "MP7")
};

/**weapon types can be assigned via blueprints*/
UENUM(BlueprintType)
enum class EFireMode :uint8 {
	SingleShot                                         UMETA(DisplayName = "SingleShot"),
	ThreeShot                                         UMETA(DisplayName = "3 contineous Shot"),
	BurstShot                                          UMETA(DisplayName = "BurstShot")
};

namespace EWeaponState
{
	enum Type
	{
		Firing,
		Idle,
		Reload,
		Droped,
	};
}

namespace EAmingMode
{
	enum Type
	{
		ADS,
		Aim,
	};
}

USTRUCT(BlueprintType)
struct FWeaponAttachmentSlot {

	GENERATED_USTRUCT_BODY()

	/** attachment slot where this attachment attach to  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AttachmentSlot")
		FName AttachmentSlotName;

	/** attachments class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AttachmentSlot")
		TSubclassOf<class ABFAttachmentBase> AttachmentClass;

	/** attachment instance associate withe this attachment class  */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AttachmentInstance")
		ABFAttachmentBase* AttachmentInstance;

	/** is this attachment slot currently be occupied?  */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AttachmentInstance")
		uint8 bisOccupied : 1;

	/** is this attachment slot currently be occupied?  */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AttachmentInstance")
		uint8 bisAvailable : 1;

	/** Default values */
	FWeaponAttachmentSlot() {
		bisOccupied = false;
		bisAvailable = true;
	}
};

USTRUCT(BlueprintType)
struct FWeaponAnim {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* ReloadAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* UnEquipAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* FirstEquipAnim;
};

USTRUCT(BlueprintType)
struct FWeaponAnim_FPS {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* ReloadAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* UnEquipAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* FirstEquipAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* ShootingAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* ADSShootingAnim;
};

USTRUCT(BlueprintType)
struct FWeaponGunAnim {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimationAsset* ReloadAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		UAnimationAsset* FireAnim;
};

USTRUCT(BlueprintType)
struct FWeaponConfigData {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int32 AmmoPerClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int32 CurrentClipAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		int32 AmmoLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		float BaseDamage;
};

USTRUCT(BlueprintType)
struct FWeaponSpreadData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFWeapon")
		float WeaponSpreadBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFWeapon")
		float WeaponSpreadIncrement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFWeapon")
		float WeaponDecressment;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFWeapon")
		float WeaponSpreadMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BFWeapon")
		float CurrentWeaponSpread;
};

/**
*   base weapon class for sub weapons to inherited from
*/
UCLASS()
class BIOFEILD_API ABFWeaponBase : public ABFInventoryItem
{
	GENERATED_UCLASS_BODY()

public:
		/** create  weapon mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeponMesh", meta = (AllowPrivateAccess = "true"))
		UBFWeaponMeshComponent*  WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeponMesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ScopeHolderMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeponMesh", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* IronSightMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ZoomingTimeLineComp", meta = (AllowePrivateAccess = "true"))
		UTimelineComponent* FOVTimeLineComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RecoilTimeLineComp", meta = (AllowePrivateAccess = "true"))
	//	UTimelineComponent* RecoilTimeLineComp;

	/** specify weapon type ,weapon type can be assigned via blueprint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Type")
		EWeaponType WeaponType;

	/**animation montage data for player who is using this weapon*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Type")
		FWeaponAnim WeaponAnim;

	/**animation montage data for player who is using this weapon*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Type")
		FWeaponAnim_FPS FPSWeaponAnim;

	/**animation data for weapon self  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Type")
		FWeaponGunAnim WeaponGunAnim;

	/** fire sound each time weapon fires */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Sound")
		USoundCue* FireSound;

	/** fire sound each time weapon fires */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Sound")
		USoundCue* SilencedFireSound;

	/** dry sound when weapon got no ammo left */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Sound")
		USoundCue* DrySound;

	/** particle effect when firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Particles")
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
		FWeaponConfigData WeaponConfigData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Spread ")
		FWeaponSpreadData WeaponSpreadData;

	/** can be initialized by blueprint and also can be changed at runtime  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Spread ")
		EFireMode  FireMode;

	/**projectile class used for this weapon  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
		TSubclassOf<ABFProjectile>ProjectileClass;

	/**projectile class used for this weapon  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<class UCameraShake>CameraShakeClass;

	/** shell eject class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet Shell")
		TSubclassOf<ABFBulletShell>ShellClass;

	/** make noise when fire weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise")
		float FireLoudness;

	/** noise effective radius */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise")
		float NoiseRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Curve")
		UCurveFloat* RecoilCurve;

	/** determine whether to play a weapon that first time equip , such as pick up a new weapon  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool bShouldPlayFirstEquipedAnim;

	
	////////////////////////////////////
	//////weapon attachments
	/////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponAttachments")
		FWeaponAttachmentSlot ScopeSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponAttachments")
		FWeaponAttachmentSlot SilencerSlot;




	///////////////////////////////////
	// user interfaces
	//////////////////////////////////////////////////////////////////////////

	/** used to show cross hair on the screen if this weapon selected as current */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UBFUserWidgetBase> CrosshairWidgetClass;

	/** texture for cross hair part */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* CrossHairTexture;

	/** widget to show when hit target as a feed back ui */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UBFUserWidgetBase> HitTargetFeedBackWidgetClass;

	/** texture for cross hair part */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* HitTargetFeedBackTexture;

	/** widget to show when hit target as a feed back ui */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UBFUserWidgetBase> WeaponInfoWidgetClass;

	/** texture for cross hair part */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* WeaponInfoWidgetClassTexture;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "UI")
		UTexture2D* WeaponIcon;

	/** widget to show when hit target as a feed back ui */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UBFUserWidgetBase> ReloadWidgetClass;

	/** texture for cross hair part */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* ReloadWidgetClassTexture;

private:
	UPROPERTY()
		uint8 bShouldShowCrossHair : 1;

protected:
	/** current weapon state */
	EWeaponState::Type WeaponState;

	/** current weapon Aiming Mode */
	EAmingMode::Type AimingMode;

	/** WeaponNames */
	EWeaponNames WeaponName;

	/** projectile type used for this weapon */
	UPROPERTY()
	EProjectileType ProjectileType;

	/** this weapon owner */
	UPROPERTY(Transient)
		ABFPlayerCharacter* WeaponOwner;

	UPROPERTY()
		ABFAttachment_Scope* WeaponScope;

	/** is selected as current weapon */
	uint8  bIsSelectedCurrent : 1;

	/** b need a extra iron sight */
	uint8 bNeedExtraIronSight : 1;

	/** b need a extra iron sight */
	uint8 bNeedExtraScopeHolder : 1;

	/** slot index the weapon hold in */
	uint8 WeaponSlotIndex;

	/** timer for equip weapon */
	FTimerHandle EquipWeaponTimerHandle;

	/** timer for unequipped weapon */
	FTimerHandle UnequipWeaponTimerHandle;

	/** timer for reload weapon */
	FTimerHandle ReloadWeaponTimerHandle;

	/** timer for reset fire */
	FTimerHandle ResetPlayerFireTimerHandle;

	/** timer for destroy self after dropped or spawned but not picked up  */
	FTimerHandle DestroySelfTimerHandle;

	/** is this weapon ready for players to picked up  */
	bool canBePickedUp;

	/** record the last fire time */
	float LastFireTime;

	/** adjust camera z off set to adapt  view location ,different weapon have different Z-off set*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		FVector AdjustADSCameraVector;

	/** is this weapon in menu mode */

	UPROPERTY()
		uint8 bIsInMenuMode : 1;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents()override;

	virtual FVector GetFinalProjectileDirection(FVector& BaseDirection);

public:
	virtual void Tick(float DeltaTime) override;

	/** general fire method called from other classes  */
	virtual void Fire();

	virtual void StopFire();

	virtual void OnWeaponADS();

	/** when weapon is not equipped ,hidden this weapon mesh and it's all available attachments  */
	virtual void HideWeapon();

	/** 	/** when weapon is not equipped ,show this weapon  mesh and it's all available attachments  */ 
	virtual void RenderWeapon();

	virtual void OnWeaponStopADS();

	/** reload weapon */
	virtual void ReloadWeapon();

	/** finish reload weapon , this should be the place to reload ammo  */
	virtual void FinishRealoadWeapon();

	/** simulate real projectile trajectory rather than line traces */
	virtual void SpawnProjectile();

	/** shoot trace to detect hit result and adjust projectile direction */
	virtual FHitResult TraceDetect();

	/** adjust projectile direction base on the trace hit and muzzle location*/
	virtual FVector AdjustProjectileDirection(FHitResult& TraceHit, FVector MuzzleLocation);

	/** add recoil effect to the owner player controller */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual void RecoilPlayer(float Value);

	/** recoil bound function for recoil player delegate */
	UFUNCTION()
		virtual void RecoilCallBackFunc();

	UFUNCTION()
		virtual void StopRecoilPlayer();

	UFUNCTION()
		virtual void StopRecoilCallBackFunc();

	/** shell effect class make the fire more realistic  */
	virtual void EjectShell();

	/** check is this weapon ready for firing before call fire function  */
	virtual bool CheckCanFire();

	/** normally ,reduce 1 ammo each shot */
	virtual void UseAmmo();

	/** check if this weapon meet the requirements to perform a reload  */
	virtual bool CheckCanReload();

	/** set given aiming mode */
	virtual void SetAmingMode(EAmingMode::Type NewAmingMode);

	/** switch between fire modes */
	virtual void ToggleFireMode();

	virtual void ToggleAimMode();

	/** set this weapon  can be pick up  */
	virtual void SetWeaponReadyForPickingUp();

	/** handle logic after this weapon is picked up */
	virtual void HandlePickUp();

	/** handle logic after this weapon is dropped  */
	virtual void HandleDroped();

	/** garbage collect this weapon  */
	virtual void SelfDestroy();

	virtual void ResetPlayerFire();

	/** return current aiming mode  */
	virtual EAmingMode::Type GetWeaponAimingMode()const { return AimingMode; }

	/** return weapon animations data  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual FWeaponAnim GetWeaponAnim()const { return WeaponAnim; }

	/** return weapon animations data  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual FWeaponAnim_FPS GetWeaponAnim_FPS()const { return FPSWeaponAnim; }

	/** return weapon animations data  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual EWeaponNames GetWeaponName()const { return WeaponName; }
	/** return weapon self animations data  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual FWeaponGunAnim GetWeaponGunAnim()const { return WeaponGunAnim; }

	/** assign weapon owner  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual void SetWeaponOwner(ABFPlayerCharacter* NewOwner);

	/** return owner of this weapon */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual ABFPlayerCharacter* GetWeaponOwner()const { return WeaponOwner; }

	/** return ammo left in pocket  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual int32 GetAmmoLeft()const { return WeaponConfigData.AmmoLeft; }

	/** return current clip ammo  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual int32 GetCurrentClipAmmo()const { return WeaponConfigData.CurrentClipAmmo; }

	/** play sound and particle effect when firing  */
	virtual void PlayFireEffects();

	virtual void SingleShot();

	/** return this weapon type  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon") 
		EWeaponType GetWeaponType()const { return WeaponType; }

	/** return this weapon config data   */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		FWeaponConfigData GetWeaponData()const { return WeaponConfigData; }

	/** set ammo left  */
	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual void SetAmmoLeft(const int32 Ammo);

	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual int32 GetClipAmmoSize()const { return WeaponConfigData.AmmoPerClip; }

	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual void SetCilpAmmoSize(int32 NewClipSize) { WeaponConfigData.AmmoPerClip = NewClipSize; }

	//UFUNCTION(BlueprintCallable, Category = "BFWeapon")
	//	virtual  float GetBurstShotEscapeTime()const;

	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual FVector GetAimDirection();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFWeapon")
		virtual EProjectileType GetProjectileType()const { return ProjectileType; }

	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual bool GetIsSelectedAsCurrent()const { return bIsSelectedCurrent; }

	UFUNCTION(BlueprintCallable, Category = "BFWeapon")
		virtual void  SetIsSelectedAsCurrent(bool bIsTure) { this->bIsSelectedCurrent = bIsTure; }

	virtual void SetWeaponSlotIndex(uint8 SlotIndex) { this->WeaponSlotIndex = SlotIndex; }

	virtual uint8 GetWeaponSlotIndex()const { return WeaponSlotIndex; }

	virtual void CreateWeaponWidgetInstances();

	virtual void SetupAttachments();

	virtual FVector GetADSCameraAdjustVector()const;

	/*******************************************************/
	/*************receive event from wepon onwer***/
	/******************************************************/

	virtual void OnWeaponEquiping();

	virtual void OnWeaponEquipingFinished();

	virtual void OnWeaponUnEquiping();

	virtual void OnWeaponUnequipingFinished();

	virtual void OnWeaponReloading();

	virtual void OnWeaponReloadingFinished();

	virtual void OnScopeEquipped();

	virtual void OnSilencerEqupped();

	virtual void SetIsMenuMode(bool IsInMenuMode);

	virtual bool GetIsInMenuMode()const { return bIsInMenuMode; }

	virtual void ReceiveDetected(class AActor* DetectedBy, class  ABFBaseCharacter* DectectedCharacter, class ABFPlayerController* DectedPlayer)override;

	virtual void AdjustCamera();

	virtual void SetLastFireTime(float TimeSeconds) { LastFireTime = TimeSeconds; }

	virtual void ResetAdjustedCamera();

	virtual FWeaponSpreadData GetWeaponSpreadData() { return WeaponSpreadData; }

	virtual float GetLastFireTime()const { return LastFireTime; }

	virtual  float GetCurrentWeaponSpread()const { return WeaponSpreadData.CurrentWeaponSpread; }

	virtual void SetCurrentWeaponSpread(float Value) { WeaponSpreadData.CurrentWeaponSpread = Value; }

	virtual  EWeaponState::Type GetWeaponState()const { return WeaponState; }

	virtual void EquipScope(ABFAttachment_Scope* ScopeToEquip);

	virtual void UnequipScope();

	virtual void EquipSilencer(ABFAttachment_Silencer* SilencerToEquip); 

	virtual void UnEquipSilencer();
};
