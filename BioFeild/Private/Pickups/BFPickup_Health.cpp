// Fill out your copyright notice in the Description page of Project Settings.

#include "BFPickup_Health.h"
#include "Character/BFBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "BFComponents/BFHealthComponent.h"

ABFPickup_Health::ABFPickup_Health(const FObjectInitializer& Objectinitializer) :Super(Objectinitializer)
{
	//InteractComp->OnComponentBeginOverlap.AddDynamic(this, &ABFPickup_Health::OnOverlapBegin);
}

void ABFPickup_Health::GiveHealthTo(ABFBaseCharacter* BeneficialCharacter)
{
	if (BeneficialCharacter)
	{
		bIsExpired = true;
		bIsActive = false;
		BeneficialCharacter->HealthComponent->ReGenerateHealth(HealthToProvide);
		PlaySoundWhenPickedUp(PickUpSound);
		Destroy();
	}
}

void ABFPickup_Health::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABFBaseCharacter* OverlapedCharacter = Cast<ABFBaseCharacter>(OtherActor);
		if (OverlapedCharacter)
		{
			const float MaxHealthValue = OverlapedCharacter->HealthComponent->GetDefaultHealth();
			const float CurrentHealthValue = OverlapedCharacter->HealthComponent->GetCurrentHealth();
			if (!OverlapedCharacter->GetCharacterIsDead() && !(MaxHealthValue == CurrentHealthValue) && !bIsExpired && !bIsActive)
			{
				GiveHealthTo(OverlapedCharacter);
			}
		}
	}
}

