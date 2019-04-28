// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class ABFImpactEffect;
class UCameraShake;
class ABFWeaponBase;
class ABFBaseCharacter;
class UDamageType;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FProjectileHitSignature, ABFProjectile*, HitProjectile, float, DamageAmount, FVector, NormalImpulse);
UCLASS(Abstract)
class BIOFEILD_API ABFProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Colision Comp", meta = (AllowPrivateAccess = "true"))
		USphereComponent*  ColisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh Comp", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileMoveMentComp", meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMoveComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, category = "Effects", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* TracerParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
		TSubclassOf<ABFImpactEffect>ImapactEffectsClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FeedBacks")
		TSubclassOf<UCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage Event")
		TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(BlueprintAssignable, Category = Events)
		FProjectileHitSignature OnProjectileHit;

protected:
	/** weapon reference that spawn this projectile */
	UPROPERTY()
		ABFWeaponBase*  WeaponOwner;

	UPROPERTY()
		/** hit result when this projectile hit something */
		FHitResult HitResult;

	UPROPERTY()
		FDamageEvent ProjectileDamageEvent;
protected:
	virtual void SpawnImpactEffect(const FHitResult& Hit);

	virtual void PostInitializeComponents()override;

	virtual float DeterminDamage(const FHitResult& HitResult);

	virtual void AddHitFeedBackToPlayerController(ABFBaseCharacter* HitCharacter);

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	virtual void SetUpWeaponOwner(ABFWeaponBase* NewOwner) { this->WeaponOwner = NewOwner; };

	virtual FHitResult GetHitResult()const { return HitResult; }

	virtual ABFWeaponBase* GetWeaponOwner()const { return  WeaponOwner; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFProjectile")
		virtual float  GetDamageTaken();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BFProjectile")
		virtual FDamageEvent GetDamageEvent()const { return ProjectileDamageEvent; }
};
