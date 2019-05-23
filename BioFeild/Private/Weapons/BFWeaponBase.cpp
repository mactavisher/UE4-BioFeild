// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeaponBase.h"
#include "BFComponents/BFWeaponMeshComponent.h"
#include "Projectile/BFProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Character/BFPlayerCharacter.h"
#include "Character/BFPlayerController.h"
#include "BFComponents//BFSkeletalMeshComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Engine/GameInstance.h"
#include "EngineMinimal.h"
#include "Engine/World.h"
#include "Animation/BFAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "UI/BFUserWidgetBase.h"
#include "Effects/BFBulletShell.h"
#include "Curves/CurveFloat.h"
#include "Attachments/BFAttachment_Silencer.h"
#include "Attachments/BFAttachment_Scope.h"
#include "Components/SceneComponent.h"
#include "BFComponents/BFInventoryComponent.h"

ABFWeaponBase::ABFWeaponBase(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
	WeaponMeshComponent = ObjectInitailizer.CreateDefaultSubobject<UBFWeaponMeshComponent>(this, TEXT("Weapon Mesh"));
	IronSightMeshComp = ObjectInitailizer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("IronSightComp"));
	FOVTimeLineComp = ObjectInitailizer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("ZoomingTimeLineComp"));
	ScopeHolderMeshComp = ObjectInitailizer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ScopeHolderComp"));
	IronSightMeshComp->SetVisibility(true);
	RootComponent = WeaponMeshComponent;
	bShouldPlayFirstEquipedAnim = false;
	canBePickedUp = false;
	bNeedExtraIronSight = false;
	bNeedExtraScopeHolder = false;
	FireLoudness = 50.0f;
	NoiseRadius = 3000.f;
	WeaponState = EWeaponState::Idle;
	AimingMode = EAmingMode::Aim;
	bIsInMenuMode = false;
	LastFireTime = 0.f;
}

// Called when the game starts or when spawned
void ABFWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CreateWeaponWidgetInstances();
	ScopeHolderMeshComp->AttachToComponent(WeaponMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponMeshComponent->ScopeHolderSocket);
	IronSightMeshComp->AttachToComponent(WeaponMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponMeshComponent->ScopeSocket);
	SetupAttachments();
	if (ScopeSlot.AttachmentInstance)
	{
		IronSightMeshComp->SetVisibility(false);
		ScopeHolderMeshComp->SetVisibility(false);
	}
	HideWeapon();
}

void ABFWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

FVector ABFWeaponBase::GetFinalProjectileDirection(FVector& BaseDirection)
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float ConeHalfAngle = FMath::DegreesToRadians(WeaponSpreadData.CurrentWeaponSpread * 0.5f);
	const FVector FinalDirection = WeaponRandomStream.VRandCone(BaseDirection, ConeHalfAngle, ConeHalfAngle);
	return FinalDirection;
}

void ABFWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFWeaponBase::Fire()
{

}

void ABFWeaponBase::StopFire()
{

}

void ABFWeaponBase::OnWeaponADS()
{

}

void ABFWeaponBase::HideWeapon()
{
	//hide attached children
	WeaponMeshComponent->SetVisibility(false);
	IronSightMeshComp->SetVisibility(false);
	TArray<USceneComponent*> WeaponAttachmentsMeshs = WeaponMeshComponent->GetAttachChildren();
	for (int32 i = 0; i < WeaponAttachmentsMeshs.Num() - 1; i++)
	{
		WeaponAttachmentsMeshs[i]->SetVisibility(false);
	}

	//hide attached weapon attachments if got any
	if (ScopeSlot.AttachmentInstance)
	{
		ScopeSlot.AttachmentInstance->MeshComp->SetVisibility(false);
	}
	if (SilencerSlot.AttachmentInstance)
	{
		SilencerSlot.AttachmentInstance->MeshComp->SetVisibility(false);
	}
}

void ABFWeaponBase::RenderWeapon()
{
	//Render weapon mesh and it's children
	WeaponMeshComponent->SetVisibility(true);
	if (bNeedExtraIronSight)
	{
		IronSightMeshComp->SetVisibility(true);
	}
	TArray<USceneComponent*> WeaponAttachmentsMeshs = WeaponMeshComponent->GetAttachChildren();
	for (int32 i = 0; i < WeaponAttachmentsMeshs.Num() - 1; i++)
	{
		WeaponAttachmentsMeshs[i]->SetVisibility(true);
	}
	//render attached weapon attachments if got any
	if (ScopeSlot.AttachmentInstance)
	{
		ScopeSlot.AttachmentInstance->MeshComp->SetVisibility(true);
		ScopeHolderMeshComp->SetVisibility(bNeedExtraScopeHolder);
		IronSightMeshComp->SetVisibility(bNeedExtraIronSight);
	}
	else 
	{
		IronSightMeshComp->SetVisibility(bNeedExtraIronSight);
	}
	if (SilencerSlot.AttachmentInstance)
	{
		SilencerSlot.AttachmentInstance->MeshComp->SetVisibility(true);
	}
}

void ABFWeaponBase::OnWeaponStopADS()
{

}

void ABFWeaponBase::SingleShot()
{
	if (CheckCanFire())
	{
		SetLastFireTime(GetWorld()->GetTimeSeconds());
		SpawnProjectile();
		if (WeaponOwner->GetCharacterIsADS())
		{
			WeaponOwner->PlayAnimMontage(FPSWeaponAnim.ADSShootingAnim, 1.0f, NAME_None);
		}
		else
		{
			WeaponOwner->PlayAnimMontage(FPSWeaponAnim.ShootingAnim, 1.0f, NAME_None);
		}
		Cast<UBFAnimInstance>(WeaponOwner->CharacterMesh->GetAnimInstance())->bisShooting = true;
		if (SilencerSlot.AttachmentInstance)
		{
			const ABFAttachment_Silencer* const SilencerInstance = Cast<ABFAttachment_Silencer>(SilencerSlot.AttachmentInstance);
			const float FireLoudnessModifier = SilencerInstance->GetFireLoudnessModifier();
			WeaponOwner->MakeNoise(FireLoudness*FireLoudnessModifier, WeaponOwner, WeaponMeshComponent->GetSocketLocation(WeaponMeshComponent->MuzzleFlashSocket), NoiseRadius*FireLoudnessModifier);
		}
		else
		{
			WeaponOwner->MakeNoise(FireLoudness, WeaponOwner, WeaponMeshComponent->GetSocketLocation(WeaponMeshComponent->MuzzleFlashSocket), NoiseRadius);
		}

		WeaponMeshComponent->PlayAnimation(WeaponGunAnim.FireAnim, false);
		PlayFireEffects();
		EjectShell();
		UseAmmo();
		if (CheckCanReload() && WeaponConfigData.CurrentClipAmmo == 0)
		{
			ReloadWeapon();
		}
	}
}

void ABFWeaponBase::ReloadWeapon()
{
	if (CheckCanReload())
	{

		UAnimMontage* ReloadAnim = nullptr;
		if (WeaponOwner->GetViewMode() == EViewMode::FPS)
		{
			ReloadAnim = FPSWeaponAnim.ReloadAnim;
		}
		if (WeaponOwner->GetViewMode() == EViewMode::TPS)
		{
			ReloadAnim = WeaponAnim.ReloadAnim;
		}
		float AnimDuration = 0.5f; //safe duration 
		AnimDuration = WeaponOwner->PlayAnimMontage(ReloadAnim, 1.f, NAME_None);
		WeaponMeshComponent->PlayAnimation(WeaponGunAnim.ReloadAnim, false);
		WeaponState = EWeaponState::Reload;
		GetWorldTimerManager().SetTimer(ReloadWeaponTimerHandle, this, &ABFWeaponBase::FinishRealoadWeapon, 1.f, false, AnimDuration);
	}
}


void ABFWeaponBase::ReceiveDetected(class AActor* DetectedBy, class ABFBaseCharacter* DectectedCharacter, class ABFPlayerController* DectedPlayer)
{
	Super::ReceiveDetected(DetectedBy, DectectedCharacter, DectedPlayer);
	WeaponMeshComponent->SetRenderCustomDepth(true);
}


void ABFWeaponBase::AdjustCamera()
{
	if (ScopeSlot.AttachmentInstance != nullptr)
	{
		ABFAttachment_Scope* CurrentScope = Cast<ABFAttachment_Scope>(ScopeSlot.AttachmentInstance);
		{
			UCameraComponent*const CharacterCamera = WeaponOwner->CameraComp;
			const FVector CameraLocation = CharacterCamera->GetRelativeTransform().GetLocation();
			CharacterCamera->SetRelativeLocation(FVector(CameraLocation.X, CameraLocation.Y, CameraLocation.Z + CurrentScope->CameraZOffSet));
			//adjust attach location
		}
	}
}

void ABFWeaponBase::ResetAdjustedCamera()
{
	if (ScopeSlot.AttachmentInstance != nullptr)
	{
		//ABFAttachment_Scope* CurrentScope = Cast<ABFAttachment_Scope>(ScopeSlot.AttachmentInstance);
		//const FVector CameraLocation = CharacterCamera->GetRelativeTransform().GetLocation();
		//{
		//	UCameraComponent* const CharacterCamera = WeaponOwner->CameraComp;
		//}
	}
}

void ABFWeaponBase::EquipScope(ABFAttachment_Scope* ScopeToEquip)
{

}

void ABFWeaponBase::UnequipScope()
{

}

void ABFWeaponBase::EquipSilencer(ABFAttachment_Silencer*SilencerToEquip)
{

}

void ABFWeaponBase::UnEquipSilencer()
{

}

void ABFWeaponBase::FinishRealoadWeapon()
{
	const int32 DeltaAmmo = WeaponConfigData.AmmoPerClip - WeaponConfigData.CurrentClipAmmo;
	if (WeaponConfigData.AmmoLeft <= DeltaAmmo)
	{
		WeaponConfigData.CurrentClipAmmo += DeltaAmmo;
		WeaponConfigData.AmmoLeft = 0;
	}
	if (WeaponConfigData.AmmoLeft > DeltaAmmo)
	{
		WeaponConfigData.CurrentClipAmmo += DeltaAmmo;
		WeaponConfigData.AmmoLeft -= DeltaAmmo;
	}
	WeaponState = EWeaponState::Idle;
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
			//DrawDebugLine(GetWorld(), WeaponMuzzleSocketTransform.GetLocation(), (WeaponMuzzleSocketTransform.GetLocation() + FinalDir*10000.f), FColor::Black, false, 2.0f, (uint8)'\000', 0.5f);
		//#endif
	}
}

FHitResult ABFWeaponBase::TraceDetect()
{
	const float TraceRange = 20000.f;
	FHitResult TraceHit(ForceInit);
	if (WeaponOwner->GetCharacterIsADS())
	{
		if (ScopeSlot.AttachmentInstance)
		{
			const ABFAttachment_Scope *const Scope = Cast<ABFAttachment_Scope>(ScopeSlot.AttachmentInstance);
			const FVector TraceStart = Scope->SceneCaptureComp->GetComponentLocation();
			const FVector TraceEnd = TraceStart + Scope->SceneCaptureComp->GetForwardVector()*TraceRange;
			GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Camera);
#if  WITH_EDITOR
			//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f);
			//DrawDebugSphere(GetWorld(), TraceHit.Location, 5.f, 5.f, FColor::Red, false, 2.0f);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("scene captrue shooting!"));
#endif
		}
	}
	else {
		const FVector TraceDirection = GetAimDirection();
		const FVector TraceStart = WeaponOwner->GetCameraLocation();
		const FVector TraceEnd = TraceStart + TraceDirection * TraceRange;
		GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Camera);
#if  WITH_EDITOR
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f);
		//DrawDebugSphere(GetWorld(), TraceHit.Location, 5.f, 5.f, FColor::Red, false, 2.0f);
#endif
	}
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
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("using adjust direction"));
#endif
		return AdjustDirection;

	}
	else {
		if (ScopeSlot.AttachmentInstance)
		{
			const ABFAttachment_Scope* const Scope = Cast<ABFAttachment_Scope>(ScopeSlot.AttachmentInstance);
			return Scope->SceneCaptureComp->GetForwardVector();
		}
		//if trace didn't hit anything ,means  target is far away enough and gonna be safe to just use the camera forward vector as projectile direction
#if WITH_EDITOR
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("using  Camera direction"));
#endif
		return GetAimDirection();
	}
}

void ABFWeaponBase::RecoilPlayer(float Value)
{
	ABFPlayerController* const PlayerController = GetWeaponOwner()->GetPlayerController();
	if (PlayerController&&RecoilCurve)
	{
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
	//if (RecoilTimeLineComp->IsPlaying())
	//{
	//	RecoilTimeLineComp->Stop();
	//}
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
	return WeaponConfigData.CurrentClipAmmo > 0 && GetWeaponOwner() != nullptr&&WeaponState != EWeaponState::Reload;
}

void ABFWeaponBase::UseAmmo()
{
	FMath::Clamp(WeaponConfigData.CurrentClipAmmo--, 0, 10000);
}

bool ABFWeaponBase::CheckCanReload()
{

	return WeaponConfigData.AmmoLeft > 0 && WeaponState != EWeaponState::Droped&&WeaponState != EWeaponState::Reload&&WeaponConfigData.AmmoPerClip != WeaponConfigData.CurrentClipAmmo;
}

void ABFWeaponBase::SetAmingMode(EAmingMode::Type NewAmingMode)
{
	this->AimingMode = NewAmingMode;
}

/** except assault rifle ,other weapon only  have single shot mode  */
void ABFWeaponBase::ToggleFireMode()
{

}

void ABFWeaponBase::ToggleAimMode()
{
	AimingMode = AimingMode == EAmingMode::Aim ? EAmingMode::ADS : EAmingMode::Aim;
}

void ABFWeaponBase::SetWeaponReadyForPickingUp()
{
	canBePickedUp = true;
}

void ABFWeaponBase::HandlePickUp()
{
	canBePickedUp = false;
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
	Cast<UBFAnimInstance>(WeaponOwner->CharacterMesh->GetAnimInstance())->bisShooting = false;
}


void ABFWeaponBase::SetWeaponOwner(ABFPlayerCharacter* NewOwner)
{
	this->WeaponOwner = NewOwner;
}

void ABFWeaponBase::PlayFireEffects()
{
	USoundCue* FireSoundToPlay = SilencerSlot.AttachmentInstance == nullptr ? FireSound : SilencedFireSound;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundToPlay, WeaponMeshComponent->GetSocketLocation(WeaponMeshComponent->MuzzleFlashSocket));
	if (MuzzleFlash)
	{
		const ABFAttachment_Silencer* SilencerInstance = Cast<ABFAttachment_Silencer>(SilencerSlot.AttachmentInstance);
		if (SilencerInstance)
		{
			const FName MuzzleFlashSocket = SilencerInstance->GetMuzzleFlashSocket();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SilencerInstance->MeshComp->GetSocketTransform(MuzzleFlashSocket), true);
		}
		else {
			const FName MuzzleFlashSocket = WeaponMeshComponent->MuzzleFlashSocket;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, WeaponMeshComponent->GetSocketTransform(MuzzleFlashSocket), true);
		}
	}
	if (CameraShakeClass)
	{
		ABFPlayerController * Controller = WeaponOwner->GetPlayerController();
		Controller->ClientPlayCameraShake(CameraShakeClass);
	}
}

void ABFWeaponBase::SetAmmoLeft(const int32 Ammo)
{
	WeaponConfigData.AmmoLeft = WeaponConfigData.AmmoLeft + Ammo;
	if (WeaponConfigData.AmmoLeft > WeaponConfigData.MaxAmmo)
	{
		WeaponConfigData.AmmoLeft = WeaponConfigData.MaxAmmo;
	}
}

FVector ABFWeaponBase::GetAimDirection()
{
	FVector CamDirection;
	FRotator CamRotation;
	ABFPlayerController* Controller = WeaponOwner->GetPlayerController();
	Controller->GetPlayerViewPoint(CamDirection, CamRotation);
	return CamRotation.Vector();
}



void ABFWeaponBase::CreateWeaponWidgetInstances()
{

}

void ABFWeaponBase::SetupAttachments()
{
	if (ScopeSlot.AttachmentClass&&ScopeSlot.bisAvailable && !ScopeSlot.bisOccupied)
	{
		ScopeSlot.AttachmentInstance = GetWorld()->SpawnActorDeferred<ABFAttachment_Scope>(ScopeSlot.AttachmentClass, GetActorTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (ScopeSlot.AttachmentInstance)
		{
			ScopeSlot.AttachmentInstance->SetWeaponOwner(this);
			//if this mesh need a extra scope holder to attach scope, then attach to the scope holder
			if (bNeedExtraScopeHolder)
			{
				ScopeSlot.AttachmentInstance->AttachToComponent(ScopeHolderMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "ScopePoint");
			}
			//if scope holder not needed ,then attach to the weapon mesh it self
			else 
			{
				ScopeSlot.AttachmentInstance->AttachToComponent(WeaponMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponMeshComponent->ScopeSocket);
			}
			ScopeSlot.bisOccupied = true;
		}
	}
	if (SilencerSlot.AttachmentClass&&SilencerSlot.bisAvailable && !SilencerSlot.bisOccupied)
	{
		SilencerSlot.AttachmentInstance = GetWorld()->SpawnActorDeferred<ABFAttachment_Silencer>(SilencerSlot.AttachmentClass, GetActorTransform(), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (SilencerSlot.AttachmentInstance)
		{
			SilencerSlot.AttachmentInstance->SetWeaponOwner(this);
			SilencerSlot.AttachmentInstance->AttachToComponent(WeaponMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponMeshComponent->MuzzleFlashSocket);
			SilencerSlot.bisOccupied = true;
		}
	}
}


FVector ABFWeaponBase::GetADSCameraAdjustVector() const
{
	return FVector::ZeroVector;
}


void ABFWeaponBase::OnWeaponEquiping()
{
	RenderWeapon();
}

void ABFWeaponBase::OnWeaponEquipingFinished()
{
	SetIsSelectedAsCurrent(true);
}

void ABFWeaponBase::OnWeaponUnEquiping()
{

}

void ABFWeaponBase::OnWeaponUnequipingFinished()
{
	WeaponOwner->InventoryComponent->WeaponSlots[GetWeaponSlotIndex()].SlotWeapon = this;//put the weapon back to inventory
	WeaponOwner->InventoryComponent->WeaponSlots[GetWeaponSlotIndex()].bIsSlotOccupied = true;
	SetIsSelectedAsCurrent(false);
	HideWeapon();
}

void ABFWeaponBase::OnWeaponReloading()
{

}

void ABFWeaponBase::OnWeaponReloadingFinished()
{

}

void ABFWeaponBase::OnScopeEquipped()
{

}

void ABFWeaponBase::OnSilencerEqupped()
{

}

void ABFWeaponBase::SetIsMenuMode(bool IsInMenuMode)
{
	this->bIsInMenuMode = IsInMenuMode;
}
