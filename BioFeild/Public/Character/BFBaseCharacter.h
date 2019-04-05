// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Character.h"
#include "BioFeild.h"
#include "BFComponents/BFSkeletalMeshComponent.h"
#include "BFComponents/BFCharacterMovementComponent.h"
#include "BFBaseCharacter.generated.h"

class UBFHealthComponent;
class UBFSkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class ABFWeaponBase;
class ABFProjectile;
class UBFVoiceComponent;
class UBFCharacterMovementComponent;
class ABFPlayerController;
class UAnimMontage;
class UBFCharacterAudioComponent;
class UBillboardComponent;

/** specify a character's current state */
namespace ECharacterState
{
	enum Type
	{
		Idle,
		Walking,
		Sprintting,
		Dead,
	};
}

/**specify character turn direction*/
UENUM(BlueprintType)
enum class ECharacterTurnType :uint8 {
	TurnLeft45,
	TurnLeft90,
	TurnRight45,
	TurnRight90,
};

/**struct to hold character turn data */
USTRUCT(BlueprintType)
struct FCharacterTurnData {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Character|TurnDirection")
		ECharacterTurnType CharacterTurnType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|TurnDirection")
		bool bShouldTurn;

	UPROPERTY(VisibleAnywhere, Category = "Character|TurnDirection")
		bool bIsTurning;

	UPROPERTY(VisibleAnywhere, Category = "Character|TurnDirection")
		float CharacterControlYawAngle;

	/** defaults value */
	FCharacterTurnData() {
		bShouldTurn = false;
		bIsTurning = false;
		CharacterControlYawAngle = 0.f;
	}
};

/**struct to store character's energy data */
USTRUCT(BlueprintType)
struct FCharacterEnergyData {

	GENERATED_USTRUCT_BODY()
     /** character's energy  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BFCharacter")
		float  Energy;

	/** character's energy  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BFCharacter")
		float  CurrentEnergy;

	/** whether or not calculate energy  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BFCharacter")
		bool  bShouldCalEnergy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter")
		float EnergyDecrement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter")
		float LowEnergyPercentage;

	/** defaults value */
	FCharacterEnergyData() {
		Energy = 100.f;
		bShouldCalEnergy = false;
		EnergyDecrement = 0.1f;
		LowEnergyPercentage = 0.5f;
		CurrentEnergy = Energy;
	}
};

/**struct to store character's heart beat data  */
USTRUCT(BlueprintType)
struct FCharacterHeartBeatData {

	GENERATED_USTRUCT_BODY()
    /** character's heart beat rate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BFCharacter")
		float  HeartBeatRate;

	/** character's current heart beat rate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BFCharacter")
		float  CurrentHeartBeatRate;

	/** should consider character heart beat rate in gameplay? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BFCharacter")
		bool  bShouldCalHeartBeatRate;

	/** should consider character heart beat rate in gameplay? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BFCharacter")
		bool bHasReachPeak;

	/** should restore heart beat rate? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BFCharacter")
		bool  bShouldRestoreHeartBeatRate;

	/** heart beat restore value  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter")
		float HeartBeatRateRestoreValue;

	/** heart beat increment value  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter")
		float HeartBeatRateIncrement;

	/** heart beat peak rate percentage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter")
		float HighHeartBeatRatePercentage;

	/** defaults value */
	FCharacterHeartBeatData() {
		HeartBeatRate = 80.f;
		bShouldCalHeartBeatRate = false;
		bShouldCalHeartBeatRate = true;
		bHasReachPeak = false;
		HeartBeatRateIncrement = 5.f;
		HighHeartBeatRatePercentage = 2.f;
		HeartBeatRateRestoreValue = 5.0f;
		CurrentHeartBeatRate = HeartBeatRate;
	}
};


USTRUCT(BlueprintType)
struct FCharacterTurnAnim {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter|Animations")
		UAnimMontage* TurnLeft45Anim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter|Animations")
		UAnimMontage* TurnLeft90Anim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter|Animations")
		UAnimMontage* TurnRight45Anim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter|Animations")
		UAnimMontage* TurnRight90Anim;
};

UCLASS()
class BIOFEILD_API ABFBaseCharacter : public ACharacter
{
		GENERATED_UCLASS_BODY()

		/*create health component for character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HealthComp", meta = (AllowPrivateAccess = "true"))
		UBFHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AudioComp", meta = (AllowPrivateAccess = "true"))
		UBFCharacterAudioComponent* CharacterVoiceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HealthComp", meta = (AllowPrivateAccess = "true"))
		UBillboardComponent* CharacterBillBoardComp;
	/*particle effect when character spawns or re_spawns */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Particle")
		UParticleSystem*SpawnFX;

	/** store character's self-turn data */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Character|Turn")
		FCharacterTurnData CharacterTurnData;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Character|Turn")
		FCharacterTurnAnim CharacterTurnAnimData;

	/** character energy data  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BFCharacter")
		FCharacterHeartBeatData CharacterHeartBeatData;

	/**character heart beat data  */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "BFCharacter")
		FCharacterEnergyData CharacterEnergyData;

protected:
	/** cached customized SkeletalMesh Comp */
	UPROPERTY()
		UBFSkeletalMeshComponent* CharacterMesh;

	/** cached customized CharacterMovement Comp */
	UPROPERTY()
		UBFCharacterMovementComponent* CharacterMoventComp;

	/** character's current state */
	ECharacterState::Type CharacterState;

	/** character's gender */
	ECharacterGender CharacterGender;

	/** timer handle for destroying this character after it dead */
	FTimerHandle DestoryCharacterTimerHandle;

	/** timer handle for destroying this character after it dead */
	FTimerHandle CharacterSelfTurnTimerHandle;

	/** projectile that hit this character */
	UPROPERTY()
		ABFProjectile* LastHitProjectile;

	/** Voice Type declare */
	EVoiceType VoiceType;

	UPROPERTY()
		bool bIsSprint;

	UPROPERTY()
		bool bWantsToSprint;

	UPROPERTY()
		bool bIsDead;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents()override;

	/** check to see if the character can sprint currently */
	virtual bool CanSprint();

	/** perform a self turn action if need to turn */
	virtual void SelfTurn();

	/** notify animation blueprint that  player start crouch */
	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void StartCrouch();

	/** notify animation blueprint that  player start jump */
	UFUNCTION(BlueprintCallable, Category = "BF|Character Movements")
		virtual void Jump()override;

	/** notify animation blueprint that  player start sprinting */
	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void Sprint();

	/** notify animation blueprint that  player is stop sprinting */
	UFUNCTION(BlueprintNativeEvent, Category = "BF|Character Movements")
		void StopSprint();


public:

	virtual void StartDestoryCharacter();

	virtual void FinishDestroyCharacter();

	virtual void SetCharacterState(ECharacterState::Type NewState);

	virtual ECharacterState::Type GetCharacterState()const { return CharacterState; }

	float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)override;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	virtual void SetHitProjectile(ABFProjectile* HitProjectile);

	virtual ABFProjectile* GetHitProjectile()const { return LastHitProjectile; }

	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);

	virtual void Suicide();

	virtual void Landed(const FHitResult& Hit);

	virtual float DecideFallingDamage(const float VelocitySize);

	virtual void KilledBy(ABFPlayerController* Killer);

	virtual void IncreaseHeartBeatRate(float Value);

	virtual void RestoreHeartBeat(float Value);

	virtual void DecreaseEnergy(float Value);

	virtual void Tick(float DeltaSeconds);

	/** if need to make the character turn it self, should tick this function every frame*/
	virtual void CalculateTurnData();

	virtual void SelfTurnFinished();

	virtual bool CheckCanSelfTurn();

	virtual UAnimMontage* GetSelfTurnAnim();

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual UBFCharacterMovementComponent*  GetBFCharacterMovement()const { return Cast<UBFCharacterMovementComponent>(GetCharacterMovement()); }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual UBFSkeletalMeshComponent*  GetBFSkeletalMesh()const {
		return Cast<UBFSkeletalMeshComponent>(GetMesh());
	};

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual UBFHealthComponent* GetHealthComponent()const { return HealthComponent; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual bool GetCharacterIsDead()const { return bIsDead; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual EVoiceType GetCharacterVoiceType()const { return VoiceType; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual void SetCharacterVoiceType(EVoiceType VoiceType);

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual bool GetIsMoving();

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual float GetHitAngle(FVector& HitDirection);

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual FVector GetCharacterXYVelocity()const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFCharacter")
		virtual ABFProjectile* GetLastProjetile()const { return LastHitProjectile; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		virtual void UpdateLastHitProjectile(ABFProjectile* HitProjectile) { this->LastHitProjectile = HitProjectile; }

	UFUNCTION()
		virtual void ReactHit();

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		bool GetCharacterIsSprint()const { return bIsSprint; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		FCharacterTurnData GetCharacterTurnData()const { return CharacterTurnData; }

	UFUNCTION(BlueprintCallable, Category = "BFCharacter")
		float GetHealthPercentage()const;

	UFUNCTION()
		virtual void OnLowHealth();

	UFUNCTION(BlueprintCallable, Category = "BFCharacter|Death")
		virtual void HandleDeath();

	UFUNCTION()
		virtual void OnHealthReduced(float ReduceAmount);
};
