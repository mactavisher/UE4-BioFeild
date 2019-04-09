// Fill out your copyright notice in the Description page of Project Settings.

#include "BFZombieAnimation.h"
#include "Bot/BFZombie.h"
#include "Bot/BFZombieController.h"

UBFZombieAnimation::UBFZombieAnimation(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

   
}

void UBFZombieAnimation::NativeInitializeAnimation()
{
	Zombie = Cast<ABFZombie>(TryGetPawnOwner());
	if (Zombie)
	{
		ZombieController = Zombie->GetZombieController();
	}
}

void UBFZombieAnimation::NativeUpdateAnimation(float DeltaSeconds)
{

}

