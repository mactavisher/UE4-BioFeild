// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeaponBase.h"
#include "BFComponents/BFWeaponMeshComponent.h"
#include "Projectile/BFProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Character/BFPlayerCharacter.h"
#include "Character/BFPlayerController.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/GameInstance.h"
#include "EngineMinimal.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "UI/BFUserWidgetBase.h"
#include "Effects/BFBulletShell.h"
#include "Curves/CurveFloat.h"
#include "Attachments/BFAttachment_Scope.h"

ABFWeaponBase::ABFWeaponBase(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMeshComponent = ObjectInitailizer.CreateDefaultSubobject<UBFWeaponMeshComponent>(this, TEXT("Weapon Mesh"));
	FOVTimeLineComp = ObjectInitailizer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("ZoomingTimeLineComp"));
	RecoilTimeLineComp = ObjectInitailizer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("RecoilTimeLineComp"));
	RootComponent = WeaponMeshComponent;
	InteractBox = ObjectInitailizer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Interact Comp"));
	InteractBox->SetBoxExtent(FVector(20.f, 20.f, 20.f), true);
	InteractBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bShouldPlayFirstEquipedAnim = false;
	ShootCount = 0;
	canBePickedUp = false;
	CurrentSpread = 0.f;
	FireLoudness = 5.0f;
	NoiseRadius = 3000.f;
	WeaponState = EWeaponState::Idle;
	AimingMode = EAmingMode::Aim;
	FireMode = EFireMode::SingleShot;
	CurrentClipAmmo = WeaponConfigData.AmmoPerClip;
	AmmoLeft = WeaponConfigData.MaxAmmo;
	bIsInMenuMode = false;
}

// Called when the game starts or when spawned
void ABFWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CreateWeaponWidgetInstances();
	SetupAttachments();
}

void ABFWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	InteractBox->OnComponentBeginOverlap.AddDynamic(this, &ABFWeaponBase::OnWeaponOverlap);
}


FVector ABFWeaponBase::GetFinalProjectileDirection(FVector& BaseDirection)
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5f);
	const FVector FinalDirection = WeaponRandomStream.VRandCone(BaseDirection, ConeHalfAngle, ConeHalfAngle);
	return FinalDirection;
}

void ABFWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFWeaponBase::Fire()
{
	if (FireMode == EFireMode::SingleShot)
	{
		SingleShot();
		WeaponOwner->OnFire();
		GetWorldTimerManager().SetTimer(ResetPlayerFireTimerHandle, this, &ABFWeaponBase::ResetPlayerFire, 1.0f, false, 0.3f);
		GetWorldTimerManager().SetTimer(WeaponSpreadTimerHandle, this, &ABFWeaponBase::DescressSpread, GetWorld()->GetDeltaSeconds(), true, 0.1f);
	}
	if (FireMode == EFireMode::ThreeShot)
	{
		if (!GetWorldTimerManager().IsTimerActive(ThreeShotTimerhanle))
		{
			GetWorldTimerManager().SetTimer(ThreeShotTimerhanle, this, &ABFWeaponBase::SingleShot, WeaponConfigData.TimeBetweenShots, true, 0.f);
		}
	}
	if (FireMode == EFireMode::BurstShot)
	{
		if (CheckCanFire())
		{
			GetWorldTimerManager().SetTimer(BurstShotTimerHandle, this, &ABFWeaponBase::SingleShot, WeaponConfigData.TimeBetweenShots, true, 0.f);
			NotifyBurst();
			if (RecoilCurve)
			{
				RecoilTimeLineComp->AddInterpFloat(RecoilCurve, RecoilEventDelegate);
			}
		}
	}
}

void ABFWeaponBase::SingleShot()
{
	if (CheckCanFire())
	{
		SpawnProjectile();
		WeaponOwner->OnFire();
		GetWorldTimerManager().SetTimer(ResetPlayerFireTimerHandle, this, &ABFWeaponBase::ResetPlayerFire, 1.0f, false, 0.05f);
		WeaponOwner->MakeNoise(FireLoudness, WeaponOwner, WeaponMeshComponent->GetSocketLocation(WeaponMeshComponent->MuzzleFlashSocket), NoiseRadius);
		WeaponMeshComponent->PlayAnimation(WeaponGunAnim.FireAnim, false);
		PlayFireEffects();
		EjectShell();
		CurrentSpread = FMath::Clamp(CurrentSpread + WeaponSpreadData.WeaponSpreadIncrement, 0.f, WeaponSpreadData.WeaponSpreadMax);
		FMath::Clamp(CurrentClipAmmo--, 0, 10000);
		if (FireMode == EFireMode::ThreeShot)
		{
			ShootCount++;
			if (ShootCount == 3)
			{
				//clear timer
				GetWorldTimerManager().ClearTimer(ThreeShotTimerhanle);
				//restore shoot count and spread  ;
				ShootCount = 0;
			}
		}
		NotifyShot();
		if (!CheckCanFire())
		{
			WeaponSpreadData.WeaponSpreadBase = 0.f;
			NotifyBurstStoped();
		}
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("current weapon spread  is %f"), CurrentSpread);
#endif
	}
	/** this means 3 shoot not finished but for some reason , weapon can't be shot ,for example ammo is 0,needs to reload */
	if (!CheckCanFire() && (ShootCount == 1 || ShootCount == 2))
	{
		//clear timer
		GetWorldTimerManager().ClearTimer(ThreeShotTimerhanle);
		//restore shoot count ;
		ShootCount = 0;
	}
}


void ABFWeaponBase::BurstShotFinished()
{
	if (FireMode == EFireMode::BurstShot)
	{
		GetWorldTimerManager().ClearTimer(BurstShotTimerHandle);
		NotifyBurstStoped();
		GetWorldTimerManager().SetTimer(WeaponSpreadTimerHandle, this, &ABFWeaponBase::DescressSpread, GetWorld()->GetDeltaSeconds(), true, 0.1f);
		CurrentSpread = 0.f;
	}
}

void ABFWeaponBase::ReloadWeapon()
{
	if (CheckCanReload())
	{
		float AnimDuration = 0.5f; //safe duration 
		AnimDuration = WeaponOwner->PlayAnimMontage(WeaponAnim.ReloadAnim, 1.f, NAME_None);
		WeaponMeshComponent->PlayAnimation(WeaponGunAnim.ReloadAnim, false);
		WeaponState = EWeaponState::Reload;
		ReceiveReloading();
		CrosshairWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		GetWorldTimerManager().SetTimer(ReloadWeaponTimerHandle, this, &ABFWeaponBase::FinishRealoadWeapon, 1.f, false, AnimDuration);
	}
}

void ABFWeaponBase::FinishRealoadWeapon()
{
	const int32 DeltaAmmo = WeaponConfigData.AmmoPerClip - CurrentClipAmmo;
	if (AmmoLeft <= DeltaAmmo)
	{
		CurrentClipAmmo += DeltaAmmo;
		AmmoLeft = 0;
	}
	if (AmmoLeft > DeltaAmmo)
	{
		CurrentClipAmmo += DeltaAmmo;
		AmmoLeft -= DeltaAmmo;
	}
	WeaponState = EWeaponState::Idle;
	ReceiveReloadFinished();
}

/** spawn a projectile each time weapon shots  */
void ABFWeaponBase::SpawnProjectile()
{
	if (ProjectileClass)
	{
		FHitResult TraceHit = TraceDetect();
		const FTransform WeaponMuzzleSocketTransform = WeaponMeshComponent->GetSocketTransform(WeaponMeshComponent->MuzzleFlashSocket);
		FVector AdjustedDirection = AdjustProjectileDirection(TraceHit, WeaponMuzzleSocketTransform.GetLocation());
		FVector FinalDir = GetFinalProjectileDirection(AdjustedDirection);
		const FTransform ProjectileSpawnTransform(FinalDir.ToOrientationRotator(), WeaponMuzzleSocketTransform.GetLocation(), FVector::OneVector);
		ABFProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ABFProjectile>(ProjectileClass, ProjectileSpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (SpawnedProjectile)
		{
			SpawnedProjectile->SetUpWeaponOwner(this);
			UGameplayStatics::FinishSpawningActor(SpawnedProjectile, ProjectileSpawnTransform);
		}
		//#if WITH_EDITOR
		//	DrawDebugLine(GetWorld(), WeaponMuzzleSocketTransform.GetLocation(), (WeaponMuzzleSocketTransform.GetLocation() + FinalDir*10000.f), FColor::Black, false, 2.0f, (uint8)'\000', 0.5f);
		//#endif
	}
}

FHitResult ABFWeaponBase::TraceDetect()
{
	const FVector TraceDirection = GetAimDirection();
	const float TraceRange = 30000.f;
	const FVector TraceStart = WeaponOwner->GetCameraLocation();
	const FVector TraceEnd = TraceStart + TraceDirection*TraceRange;
	FHitResult TraceHit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	//#if  WITH_EDITOR
	//	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f);
	//	DrawDebugSphere(GetWorld(), TraceHit.Location, 5, 5, FColor::Red,false,2.0f);
	//#endif
	return TraceHit;
}

FVector ABFWeaponBase::AdjustProjectileDirection(FHitResult & TraceHit, FVector MuzzleLocation)
{
	if (TraceHit.bBlockingHit)
	{
		const FVector HitLocation = TraceHit.ImpactPoint;
		FVector AdjustDirection = HitLocation - MuzzleLocation;
		const float DotProduct = FVector::DotProduct(HitLocation, MuzzleLocation);
		//DrawDebugLine(GetWorld(), MuzzleLocation, (MuzzleLocation + AdjustDirection * 2000), FColor::Red, false, 2.f, (uint8)'\000', 0.5f);
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("using adjust direction"));
#endif
		return AdjustDirection;

	}
	else {
		//if trace didn't hit anything ,means  target is far away enough and gonna be safe to just use the camera forward vector as projectile direction
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("using  Camera direction"));
#endif
		return GetAimDirection();
	}
}

void ABFWeaponBase::RecoilPlayer(float Value)
{
	ABFPlayerController* const PlayerController = GetWeaponOwner()->GetPlayerController();
	if (PlayerController&&RecoilCurve)
	{
		Value = RecoilCurve->GetFloatValue(GetWorld()->GetDeltaSeconds());
		PlayerController->AddPitchInput(Value);
	}
}

void ABFWeaponBase::RecoilCallBackFunc()
{
 
}

void ABFWeaponBase::StopRecoilPlayer()
{

}

void ABFWeaponBase::StopRecoilCallBackFunc()
{
	if (RecoilTimeLineComp->IsPlaying())
	{
		RecoilTimeLineComp->Stop();
	}
}

void ABFWeaponBase::EjectShell()
{
	FTransform  ShellSocketTransform = WeaponMeshComponent->GetSocketTransform(WeaponMeshComponent->ShellEjectSocket);
	FTransform AdjustedTranform(ShellSocketTransform.GetLocation().ForwardVector.ToOrientationRotator(), ShellSocketTransform.GetLocation(), FVector(1.0f, 1.0f, 1.0f));
	if (ShellClass)
	{
		ABFBulletShell* EjectedShell = GetWorld()->SpawnActorDeferred<ABFBulletShell>(ShellClass, ShellSocketTransform);
		if (EjectedShell)
		{
			EjectedShell->SetWeaponEjector(this);
			UGameplayStatics::FinishSpawningActor(EjectedShell, ShellSocketTransform);
		}
	}
}

bool ABFWeaponBase::CheckCanFire()
{
	return CurrentClipAmmo > 0 && GetWeaponOwner() != nullptr&&WeaponState != EWeaponState::Reload;
}

bool ABFWeaponBase::CheckCanReload()
{
	return AmmoLeft > 0 && WeaponState != EWeaponState::Droped&&WeaponState != EWeaponState::Reload&&WeaponConfigData.AmmoPerClip != CurrentClipAmmo;
}

void ABFWeaponBase::SetAmingMode(EAmingMode::Type NewAmingMode)
{
	this->AimingMode = NewAmingMode;
}

/** except assault rifle ,other weapon only  have single shot mode  */
void ABFWeaponBase::ToggleFireMode()
{
	if (WeaponType == EWeaponType::AssaultRifle)
	{
		/** switch between those three fire modes  */
		FireMode = FireMode == EFireMode::SingleShot ? (FireMode == EFireMode::BurstShot ? EFireMode::ThreeShot : EFireMode::BurstShot) : EFireMode::SingleShot;
	}

	/** do not compile and display those debug message on shipping builds */
#if WITH_EDITOR
	if (FireMode == EFireMode::SingleShot)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("SingleShot"));
	}
	if (FireMode == EFireMode::ThreeShot)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("ThreeShot"));
	}
	if (FireMode == EFireMode::BurstShot)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("BurstShot"));
	}
# endif 
}

void ABFWeaponBase::SetWeaponReadyForPickingUp()
{
	canBePickedUp = true;
	InteractBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABFWeaponBase::HandlePickUp()
{
	canBePickedUp = false;
	InteractBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/** do not destroy this weapon if picked up by player  */
	if (GetWorldTimerManager().IsTimerActive(DestroySelfTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DestroySelfTimerHandle);
	}
}

void ABFWeaponBase::HandleDroped()
{
	SetWeaponReadyForPickingUp();
	GetWorldTimerManager().SetTimer(DestroySelfTimerHandle, this, &ABFWeaponBase::SelfDestroy, 1.0f, false, 20.f);
}

void ABFWeaponBase::SelfDestroy()
{
	this->Destroy(true);
}

void ABFWeaponBase::ResetPlayerFire()
{
	WeaponOwner->OnResetFire();
}

void ABFWeaponBase::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	const ABFPlayerCharacter* const OverlapedPlayer = Cast<ABFPlayerCharacter>(OtherActor);
	if (OverlapedPlayer && canBePickedUp)
	{
		OnWeaponBeginOverlap.Broadcast(this);
	}
}

void ABFWeaponBase::SetWeaponOwner(ABFPlayerCharacter* NewOwner)
{
	this->WeaponOwner = NewOwner;
}

void ABFWeaponBase::PlayFireEffects()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, WeaponMeshComponent->GetSocketLocation(WeaponMeshComponent->MuzzleFlashSocket));
	}
	if (MuzzleFlash)
	{
		const FName MuzzleFlashSocket = WeaponMeshComponent->MuzzleFlashSocket;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, WeaponMeshComponent->GetSocketTransform(MuzzleFlashSocket), true);
		//UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMeshComponent, MuzzleFlashSocket, WeaponMeshComponent->GetSocketLocation(MuzzleFlashSocket), WeaponMeshComponent->GetSocketRotation(MuzzleFlashSocket), EAttachLocation::SnapToTarget, true);
	}
	if (CameraShakeClass)
	{
		ABFPlayerController * Controller = WeaponOwner->GetPlayerController();
		Controller->ClientPlayCameraShake(CameraShakeClass);
	}
}

//void ABFWeaponBase::SpawnEmptyMagazine()
//{
	//FActorSpawnParameters MagazineMeshSpawnParams();
	//MagazineMeshSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//if (EmptyMagazineClass)
	//{
//		ABFEmptyMagazine* EmptyMagazine = GetWorld()->SpawnActor<ABFEmptyMagazine>(EmptyMagazineClass);
	//}
//}

void ABFWeaponBase::SetAmmoLeft(const int32 Ammo)
{
	FMath::Clamp(AmmoLeft + Ammo, 0, WeaponConfigData.MaxAmmo);
}

float ABFWeaponBase::GetBurstShotEscapeTime() const
{
	if (GetWorldTimerManager().IsTimerActive(BurstShotTimerHandle))
	{
		return GetWorldTimerManager().GetTimerElapsed(BurstShotTimerHandle);
	}
	return 0.f;
}

FVector ABFWeaponBase::GetAimDirection()
{
	FVector CamDirection;
	FRotator CamRotation;
	ABFPlayerController* Controller = WeaponOwner->GetPlayerController();
	Controller->GetPlayerViewPoint(CamDirection, CamRotation);
	return CamRotation.Vector();
}

void ABFWeaponBase::IncreaseSpread()
{

}

void ABFWeaponBase::DescressSpread()
{
	CurrentSpread -= WeaponSpreadData.WeaponDecressment;
	if (CurrentSpread < 0)
	{
		CurrentSpread = 0;
		GetWorldTimerManager().ClearTimer(WeaponSpreadTimerHandle);
	}
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("current weapon spread  is %f"), CurrentSpread);
#endif
}

void ABFWeaponBase::CreateWeaponWidgetInstances()
{
	if (WeaponOwner&&!bIsInMenuMode)
	{
		if (CrosshairWidgetClass)
		{
			CrosshairWidgetInstance = CreateWidget<UBFUserWidgetBase>(WeaponOwner->GetPlayerController(), CrosshairWidgetClass);
		}
		if (HitTargetFeedBackWidgetClass)
		{
			HitTargetFeedBackWidgetInstance = CreateWidget<UBFUserWidgetBase>(WeaponOwner->GetPlayerController(), HitTargetFeedBackWidgetClass);
		}
		if (WeaponInfoWidgetClass)
		{
			HitTargetFeedBackWidgetInstance = CreateWidget<UBFUserWidgetBase>(WeaponOwner->GetPlayerController(), WeaponInfoWidgetClass);
		}
		if (ReloadWidgetClass)
		{
			ReloadingWidgetInstance = CreateWidget<UBFUserWidgetBase>(WeaponOwner->GetPlayerController(), ReloadWidgetClass);
		}
	}
}

void ABFWeaponBase::SetupAttachments()
{
	if (ScopeClass)
	{
		WeaponScope = GetWorld()->SpawnActorDeferred<ABFAttachment_Scope>(ScopeClass, GetActorTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (WeaponScope)
		{
			WeaponScope->SetWeaponOwner(this);
			WeaponScope->AttachToComponent(WeaponMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponMeshComponent->ScopeSocket);
		}
	}
}

void ABFWeaponBase::OnCollect()
{

}

void ABFWeaponBase::ReceiveReloadFinished()
{
	if (!bIsInMenuMode)
	{
		CrosshairWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABFWeaponBase::ReceiveReloading()
{
	if (!bIsInMenuMode)
	{
		CrosshairWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABFWeaponBase::ReceiveEquiping()
{
	//by default nothing to do currently , but this event will get received
}

void ABFWeaponBase::ReciveFinishEquiping()
{
	if (!bIsInMenuMode)
	{
		if (CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->AddToViewport(0);
		}
		if (ReloadingWidgetInstance)
		{
			ReloadingWidgetInstance->AddToViewport(0);
		}
		if (HitTargetFeedBackWidgetInstance)
		{
			HitTargetFeedBackWidgetInstance->AddToViewport(0);
		}
		if (WeaponInfoWidgetInstance)
		{
			WeaponInfoWidgetInstance->AddToViewport(0);
		}
	}
}

void ABFWeaponBase::ReceiveUnequiping()
{
	if (!bIsInMenuMode)
	{
		if (CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->RemoveFromViewport();
		}
		if (ReloadingWidgetInstance)
		{
			ReloadingWidgetInstance->RemoveFromViewport();
		}
		if (HitTargetFeedBackWidgetInstance)
		{
			HitTargetFeedBackWidgetInstance->RemoveFromViewport();
		}
		if (WeaponInfoWidgetInstance)
		{
			WeaponInfoWidgetInstance->RemoveFromViewport();
		}
	}
}

void ABFWeaponBase::SetIsMenuMode(bool IsInMenuMode)
{
	this->bIsInMenuMode = IsInMenuMode;
}
