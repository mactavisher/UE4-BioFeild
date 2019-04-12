// Fill out your copyright notice in the Description page of Project Settings.

#include "BFSkeletalMeshComponent.h"
#include "Animation/BFAnimInstance.h"
#include "DrawDebugHelpers.h"
#include"Projectile/BFProjectile.h"
#include "Character/BFPlayerController.h"
#include"Character/BFBaseCharacter.h"
#include"Character/BFPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"


UBFSkeletalMeshComponent::UBFSkeletalMeshComponent(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	SetAnimInstanceClass(UBFAnimInstance::StaticClass());
	bCanPerformStep = false;
	PrimaryComponentTick.bCanEverTick = true;
	StepLoudnessBase = 1.0f;
	StepNoiseEffectiveRadius = 100.f;
	bTickDetectTrace = true;
	this->OnComponentHit.AddDynamic(this, &UBFSkeletalMeshComponent::HandleHit);
	SocketNames.CurrentWeaponPistolSocket = TEXT("CurrentHandGunBaseSocket");
	SocketNames.CurrentWeaponRifleSocket = TEXT("CurrentRifleBaseSocket");
	SocketNames.RifleHolsterSocket = TEXT("Socket_ARHolster");
	SocketNames.PistolHolsterSocket = TEXT("SecondaryWeaponAttachingSocket");
	SocketNames.FootSocket = TEXT("FootSocket");
}

/**
*    simulate foot step sound or other FX effects
*    use a 10 unit long line trace shoot down from foot  to decide the surface type foot step on
*/
FHitResult UBFSkeletalMeshComponent::OnLeftStep()
{
	FHitResult LeftFootStepHit(ForceInit);
	const FVector FootTraceStartLocation = GetSocketLocation(SocketNames.LeftStepSocket);
	const FVector FootTraceEndLocation(FootTraceStartLocation.X, FootTraceStartLocation.Y, FootTraceStartLocation.Z - 10.f);
	GetWorld()->LineTraceSingleByChannel(LeftFootStepHit, FootTraceStartLocation, FootTraceEndLocation, ECC_Visibility);
	OwnerCharacter->MakeNoise(StepLoudnessBase, OwnerCharacter, FootTraceStartLocation, StepNoiseEffectiveRadius);
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), FootTraceStartLocation, FootTraceEndLocation, FColor::Green, false, 1.0f);
#endif
	return LeftFootStepHit;
}

/**
*    simulate foot step sound or other FX effects
*    use a 10 unit long line trace shoot from foot  to decide the surface type foot step on
*/
FHitResult UBFSkeletalMeshComponent::OnRightStep()
{
	FHitResult RightFootStepHit(ForceInit);
	const FVector FootTraceStartLocation = GetSocketLocation(SocketNames.RightStepSocket);
	const FVector FootTraceEndLocation(FootTraceStartLocation.X, FootTraceStartLocation.Y, FootTraceStartLocation.Z - 10.f);
	GetWorld()->LineTraceSingleByChannel(RightFootStepHit, FootTraceStartLocation, FootTraceEndLocation, ECC_Visibility);
	OwnerCharacter->MakeNoise(StepLoudnessBase, OwnerCharacter, FootTraceStartLocation, StepNoiseEffectiveRadius);
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), FootTraceStartLocation, FootTraceEndLocation, FColor::Green, false, StepNoiseEffectiveRadius);
#endif
	return RightFootStepHit;
}

void UBFSkeletalMeshComponent::LineTraceFromPevils()
{

}

void UBFSkeletalMeshComponent::StepOnDetectTrace1()
{
	FHitResult TraceHit(ForceInit);
	const FVector FootLocation = GetSocketLocation(SocketNames.FootSocket);
	const FVector TraceDirection = GetSocketTransform(SocketNames.FootSocket).GetRotation().GetForwardVector();
	const FVector TraceEndLocation = FootLocation + TraceDirection * 80;
	bool bisHit = GetWorld()->LineTraceSingleByChannel(TraceHit, FootLocation, TraceEndLocation, ECC_Visibility);
	if (bisHit)
	{
		StepOnDetect1TraceHit = TraceHit;
	}
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), FootLocation, TraceEndLocation, FColor::Green, false, 0.1f, (uint8)'\000', 1.f);
#endif
}

void UBFSkeletalMeshComponent::StepOnDetectTrace2()
{
	FHitResult TraceHit(ForceInit);
	const FVector FootLocation = GetSocketLocation(SocketNames.FootSocket);
	const FVector TraceDirection = GetSocketTransform(SocketNames.FootSocket).GetRotation().GetForwardVector();
	const FVector TraceEndLocation = FootLocation + TraceDirection * 80;
	bool bisHit = GetWorld()->LineTraceSingleByChannel(TraceHit, FootLocation, TraceEndLocation, ECC_Visibility);
	if (bisHit)
	{
		StepOnDectct2TraceHit = TraceHit;
	}

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), FootLocation, TraceEndLocation, FColor::Green, false, 0.1f, (uint8)'\000', 1.f);
#endif
}


float UBFSkeletalMeshComponent::CalculatePitchOffSet()
{
	float Pitch = 0.f;
	if (OwnerCharacter)
	{
		ABFPlayerCharacter* PlayerCharacter = Cast<ABFPlayerCharacter>(OwnerCharacter);
		if (PlayerCharacter)
		{
			ABFPlayerController* PC = PlayerCharacter->GetPlayerController();
			if (PC)
			{
				const FRotator ControlRot = PC->GetControlRotation();
				const FRotator MeshRot = OwnerCharacter->GetActorRotation();
				Pitch = UKismetMathLibrary::NormalizedDeltaRotator(ControlRot, MeshRot).Pitch;
			}
		}
	}
	return Pitch;
}

void UBFSkeletalMeshComponent::ReceiveProjectileHit(ABFProjectile* HitProjectile, float DamageAmount, FVector NormalImpulse)
{
	this->HitProjectile = HitProjectile;
	const FDamageEvent DamageEvent = HitProjectile->GetDamageEvent();
	OwnerCharacter->TakeDamage(DamageAmount, DamageEvent, HitProjectile->GetWeaponOwner()->GetWeaponOwner()->GetPlayerController(), HitProjectile);
	if (OwnerCharacter->GetCharacterIsDead())
	{
		this->AddImpulseAtLocation(HitProjectile->GetVelocity()*1.5f, HitProjectile->GetActorLocation(), NAME_None);
	}
}

bool UBFSkeletalMeshComponent::CalculateCanStepOn()
{
	if (StepOnDetect1TraceHit.bBlockingHit && !StepOnDectct2TraceHit.bBlockingHit)
	{
		return bCanPerformStep = true;
		OnCanStep.Broadcast(StepOnDetect1TraceHit, StepOnDectct2TraceHit);
	}
	else
	{
		return bCanPerformStep = false;
	}
}

void UBFSkeletalMeshComponent::HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABFProjectile* HitProjectile = Cast<ABFProjectile>(OtherActor);
	if (HitProjectile)
	{
		const FHitResult HitResult = HitProjectile->GetHitResult();
		//HitProjectile->OnProjectileHit.AddDynamic(this, &UBFSkeletalMeshComponent::ReceiveProjectileHit);
		HitProjectile->OnProjectileHit.AddDynamic(this, &UBFSkeletalMeshComponent::ReceiveProjectileHit);
	}
}

void UBFSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentAnimInstance = Cast<UBFAnimInstance>(GetAnimInstance());
}

UBFAnimInstance* UBFSkeletalMeshComponent::GetCurrentAnimInstance()
{
	if (CurrentAnimInstance)
	{
		return CurrentAnimInstance;
	}
	else {
		CurrentAnimInstance = Cast<UBFAnimInstance>(GetAnimInstance());
		return CurrentAnimInstance;
	}
}

void UBFSkeletalMeshComponent::DeactivateDetectTrace()
{
	bTickDetectTrace = false;
}

void UBFSkeletalMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bTickDetectTrace)
	{
		StepOnDetectTrace1();
		StepOnDetectTrace2();
		if (bCanPerformStep)
		{
			/** send event dispatcher to  he player character when character can perform a step on move  */
			OnCanStep.Broadcast(StepOnDetect1TraceHit, StepOnDectct2TraceHit);
		}
	}
}

void UBFSkeletalMeshComponent::SetHitProjectile(ABFProjectile* HitProjectile)
{

}

void UBFSkeletalMeshComponent::SetHitOwnerCharacter(ABFBaseCharacter* Owner)
{

}

void UBFSkeletalMeshComponent::SetCharacterOwner(ABFBaseCharacter* Owner)
{
	OwnerCharacter = Owner;
}
