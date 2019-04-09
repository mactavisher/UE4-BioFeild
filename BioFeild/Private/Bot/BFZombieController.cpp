// Fill out your copyright notice in the Description page of Project Settings.

#include "BFZombieController.h"
#include "Bot/BFZombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BFPlayerCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "EngineMinimal.h"
#include "Engine/TargetPoint.h"
#include "Engine/Engine.h"
#include "Pickups/BFPickup_Ammo.h"
#include "Pickups/BFPickupBase.h"
#include "TimerManager.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/NavigationPath.h"
#include "Pickups/BFPickup_Health.h"
#include "DrawDebugHelpers.h"

ABFZombieController::ABFZombieController(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	BrainComponent = BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("Behaviour Tree"));
	PrimaryActorTick.bCanEverTick = true;
}

void ABFZombieController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABFZombieController::BeginPlay()
{
	Super::BeginPlay();
}

void ABFZombieController::WalkTo()
{
	OnWalk();
}

void ABFZombieController::ChargeTo()
{
	OnCharge();
}

void ABFZombieController::CrawlerTo()
{
	onCrawler();
}

void ABFZombieController::AttackPlayer()
{
	const bool bisZombieAttacking = PossessedZombie->GetIsZombieAttacking();
	if (!bisZombieAttacking&&bWantsToAttack)
	{
		PossessedZombie->SetIsZombieAttacking(true);
		PossessedZombie->PlayAnimMontage(PossessedZombie->AttackAnim, 1.0f, NAME_None);
		OnAttack();
	}
}

void ABFZombieController::StopMovement()
{
	PossessedZombie->SetZombieCurrentState(EZombieState::Idle);
	Super::StopMovement();
	OnStopMoveMent();
}

bool ABFZombieController::DecideToTrackingPlayer()
{
	int32 RamdonNum = FMath::RandHelper(2);
	if (RamdonNum == 2)
	{
		LostPlayer();
	}
	return true;
}

void ABFZombieController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FocusTime = 1.5f;
	AttackableRadius = 20.f;
	LostSightTime = 10.f;
	bLostTarget = true;
}

void ABFZombieController::LostPlayer()
{
	PlayerEnemy = nullptr;
	bLostTarget = true;
	if (GetWorldTimerManager().TimerExists(LostPlayerTimerHanle) && GetWorldTimerManager().IsTimerActive(LostPlayerTimerHanle))
	{
		GetWorldTimerManager().ClearTimer(LostPlayerTimerHanle);
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Holyshit,targe lost"));
#endif
	}
}

FVector ABFZombieController::FindNextMoveToLocation()
{
	TArray<AActor*> AllWaypoints;
	UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), AllWaypoints);
	int32 NumberOfWayPoint = AllWaypoints.Num();
	int32 SelectedPoint = FMath::RandHelper(NumberOfWayPoint - 1);
	FVector SelectedPointLocation = AllWaypoints[SelectedPoint]->GetActorLocation();
	DesireDestination = SelectedPointLocation;
	return SelectedPointLocation;
}

FVector ABFZombieController::FindeNextSearchLocation()
{
	return FVector(0.f, 0.f, 0.f);
}

void ABFZombieController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	PossessedZombie = Cast<ABFZombie>(InPawn);
	if (PossessedZombie)
	{
		PossessedZombie->SetZombieController(this);
		PossessedZombie->SetZombieCurrentState(EZombieState::Idle);
		PossessedZombie->OnSeePlayer.AddDynamic(this, &ABFZombieController::ReceiveZombieSeePlayer);
		PossessedZombie->OnHearPlayer.AddDynamic(this, &ABFZombieController::ReceiveZombieHearPlayer);
		PossessedZombie->OnZombieDead.AddDynamic(this, &ABFZombieController::OnZombieDead);
		if (PossessedZombie->ZombieBehaviour)
		{
			RunBehaviorTree(PossessedZombie->ZombieBehaviour);
		}
	}
}

void ABFZombieController::UnPossess()
{
	Super::UnPossess();
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("zombie is dead, and i do not want to controll a dead zombie..!"));
#endif
}

bool ABFZombieController::CanSeeEnemy() const
{
	if (!PlayerEnemy)
	{
		return false;
	}
	return LineOfSightTo(PlayerEnemy, PlayerEnemy->GetPawn()->GetActorLocation(), true);
}

void ABFZombieController::DiscardCurrentPlayer()
{
	PlayerEnemy = nullptr;
	bLostTarget = true;
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Holyshit,targe lost"));
#endif
}

void ABFZombieController::SpawnPickUps()
{
	FActorSpawnParameters PickupSpawnParameter;
	PickupSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	PickupSpawnParameter.Instigator = this->PossessedZombie;
	if (!AmmoRewardClass&&HealthRewardClass)
	{
		GetWorld()->SpawnActor<ABFPickup_Health>(HealthRewardClass, PossessedZombie->GetActorTransform(), PickupSpawnParameter);
	}
	if (AmmoRewardClass && !HealthRewardClass)
	{
		GetWorld()->SpawnActor<ABFPickup_Ammo>(AmmoRewardClass, PossessedZombie->GetActorTransform(), PickupSpawnParameter);
	}
	if (AmmoRewardClass && AmmoRewardClass)
	{
		const int32 RandomInteger = FMath::RandHelper(5);
		if (RandomInteger == 2)
		{
			GetWorld()->SpawnActor<ABFPickup_Health>(HealthRewardClass, PossessedZombie->GetActorTransform(), PickupSpawnParameter);
		}
		if (RandomInteger == 4)
		{
			GetWorld()->SpawnActor<ABFPickup_Ammo>(AmmoRewardClass, PossessedZombie->GetActorTransform(), PickupSpawnParameter);
		}
	}
}

void ABFZombieController::OnZombieDead()
{
	const FName BlackBoardKey = TEXT("IsSelfDead");
	GetBlackboardComponent()->SetValueAsBool(BlackBoardKey, true);
	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	UnPossess();
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("zombie is dead"));
#endif
}

void ABFZombieController::ReceiveZombieHearPlayer(ABFPlayerCharacter* PlayerCharacter, const FVector& Location, float Volume)
{
	ABFPlayerController* Player = PlayerCharacter->GetPlayerController();
	//only set PlayerEnemy if it null or Player is not same as PlayerEnemy 
	GetBlackboardComponent()->SetValueAsBool("HasHearNoise", true);
	GetBlackboardComponent()->SetValueAsVector("NoiseLocation", Player->GetPoccessedPlayerCharacter()->GetActorLocation());
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("I hear something................................................!"));
#endif
	if (PlayerEnemy == nullptr&&PlayerEnemy != Player)
	{
		PlayerEnemy = Player;
		DecideToTrackingPlayer();
	}
}


void ABFZombieController::ReceiveZombieSeePlayer(ABFPlayerController* PlayerController, ABFPlayerCharacter* PlayerPawn, FVector Location)
{
	GetWorldTimerManager().SetTimer(LostPlayerTimerHanle, this, &ABFZombieController::LostPlayer, 1.0f, false, 2.f);
	//only set PlayerEnemy if it null or Player is not same as PlayerEnemy 
	if (PlayerEnemy == nullptr&&PlayerEnemy != PlayerController && PlayerController != nullptr)
	{
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("I see something!"));
#endif
		//update blackboard value is validated
		PlayerEnemy = PlayerController;
		const FName HasEnemyKey = TEXT("CanSeeEnemy");
		const FName EnemyLocationKey = TEXT("EnemyLocation");
		const FName EnemyKey = TEXT("Enemy");
		GetBlackboardComponent()->SetValueAsBool(HasEnemyKey, true);
		GetBlackboardComponent()->SetValueAsObject(EnemyKey, PlayerController);
		GetBlackboardComponent()->SetValueAsVector(EnemyLocationKey, PlayerPawn->GetActorLocation());
	}
}

void ABFZombieController::FindPathPointsToMove()
{
	if (DesireDestination != FVector::ZeroVector)
	{
		UNavigationSystem* Navigation = GetWorld()->GetNavigationSystem();
		if (Navigation)
		{
			const UNavigationPath* const Path = Navigation->FindPathToLocationSynchronously(GetWorld(), PossessedZombie->GetActorLocation(), DesireDestination, PlayerEnemy->GetPoccessedPlayerCharacter(), GetDefaultNavigationFilterClass());
			this->PathPoints = Path->PathPoints;
			const int32 RandomNum = FMath::RandRange(1, this->PathPoints.Num() - 1);
			if (PathPoints.Num() > 1)
			{
				NextMoveToLocation = this->PathPoints[RandomNum];
			}
			// if play with editor , then draw debug sphere to visually present the paths to location, and the chosen path point will display in Red
#if WITH_EDITOR
			FString Debugmessage;
			Debugmessage.AppendInt(this->PathPoints.Num());
			Debugmessage.Append(TEXT("Current Choose")).AppendInt(RandomNum);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, Debugmessage);
			for (int32 i = 0; i <= this->PathPoints.Num() - 1; i++)
			{
				if (i == RandomNum)
				{
					DrawDebugSphere(GetWorld(), NextMoveToLocation, 10.f, 5, FColor::Red, false, 2.0f);
				}
				else {
					DrawDebugSphere(GetWorld(), this->PathPoints[i], 10.f, 5, FColor::Green, false, 2.0f);
				}
			}
#endif
		}
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("You may forgot to put a navigation bounds volume to your level"));
#endif
	}
}



