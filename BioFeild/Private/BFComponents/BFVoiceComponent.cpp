// Fill out your copyright notice in the Description page of Project Settings.

#include "BFVoiceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Voice/BFVoice.h"
#include "Character/BFBaseCharacter.h"
#include "BFComponents/BFSkeletalMeshComponent.h"

// Sets default values for this component's properties
UBFVoiceComponent::UBFVoiceComponent(const FObjectInitializer&ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UBFVoiceComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterMesh = CharacterOwner->GetBFSkeletalMesh();
}

void UBFVoiceComponent::SetOwner(ABFBaseCharacter* NewOwner)
{
	CharacterOwner = NewOwner;
}

void UBFVoiceComponent::Speak(EVoiceType VoiceType)
{
	const FTransform MouthTransform = CharacterMesh->GetSocketTransform(CharacterMesh->SocketNames.MouthSocket);
	ABFVoice* Voice = nullptr;
	if (CharacterGender == ECharacterGender::male)
	{
		ABFVoice * Voice = GetWorld()->SpawnActorDeferred<ABFVoice>(MaleSoundClass, MouthTransform, CharacterOwner, CharacterOwner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	}
	if (CharacterGender == ECharacterGender::female)
	{
		ABFVoice * Voice = GetWorld()->SpawnActorDeferred<ABFVoice>(FeMaleSoundClass, MouthTransform, CharacterOwner, CharacterOwner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	}
	if (Voice)
	{
		Voice->SetCharacter(CharacterOwner);
		Voice->SetVoiceType(VoiceType);
		UGameplayStatics::FinishSpawningActor(Voice, MouthTransform);
	}
}

void UBFVoiceComponent::SetCharacterGender(ECharacterGender InGender)
{
	CharacterGender = InGender;
}

