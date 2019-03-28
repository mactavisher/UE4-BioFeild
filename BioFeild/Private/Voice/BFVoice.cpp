// Fill out your copyright notice in the Description page of Project Settings.

#include "BFVoice.h"
#include "Sound/SoundCue.h"
#include "Character/BFBaseCharacter.h"
#include "BFComponents/BFSkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABFVoice::ABFVoice(const FObjectInitializer&ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 2.5f;
}

void ABFVoice::PostInitializeComponents()
{
	UBFSkeletalMeshComponent* const Mesh = Character->GetBFSkeletalMesh();
	if (SelectSoundByVoiceType())
	{
		UGameplayStatics::SpawnSoundAttached(SelectSoundByVoiceType(), Mesh, NAME_None, Mesh->GetSocketLocation(Mesh->SocketNames.MouthSocket), EAttachLocation::SnapToTarget, false, 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
	}
}

USoundCue* ABFVoice::SelectSoundByVoiceType()
{
	USoundCue* SoundToSpeak = nullptr;
	switch (VoiceType)
	{
	default: SoundToSpeak = nullptr;
	case EVoiceType::Death: SoundToSpeak = DeathVoice; break;
	case EVoiceType::FallingFormHeight:SoundToSpeak = FallingFromHeightVoice; break;
	case EVoiceType::Hurts:SoundToSpeak = HurtsVoice; break;
	}
	return SoundToSpeak;
}

void ABFVoice::SetVoiceType(EVoiceType VoiceType)
{
	VoiceType = VoiceType;
}

void ABFVoice::SetCharacter(ABFBaseCharacter* Character)
{
	Character = Character;
}

