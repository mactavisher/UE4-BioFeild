// Fill out your copyright notice in the Description page of Project Settings.

#include "BFCharacterAudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Character/BFBaseCharacter.h"
#include "BFComponents/BFSkeletalMeshComponent.h"

UBFCharacterAudioComponent::UBFCharacterAudioComponent(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBFCharacterAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterMesh = CharacterOwner->GetBFSkeletalMesh();
}

void UBFCharacterAudioComponent::SetOwner(ABFBaseCharacter* NewOwner)
{
	CharacterOwner = NewOwner;
}

USoundCue* UBFCharacterAudioComponent::GetPlaySound(EVoiceType VoiceType)
{
	USoundCue* SoundToPlay = nullptr;
	if(CharacterGender==ECharacterGender::male)
	switch (VoiceType)
	{
	case EVoiceType::Hurts:SoundToPlay = MaleSoundData.HurtsVoice;break;
	case EVoiceType::HighHeartBeatRate:SoundToPlay = MaleSoundData.HurtsVoice; break;
	case EVoiceType::LowEnergy:SoundToPlay = nullptr;break;
	case EVoiceType::FallingFormHeight:SoundToPlay = MaleSoundData.FallingFromHeightVoice;break;
	case EVoiceType::Death:SoundToPlay = MaleSoundData.DeathVoice;break;
	case EVoiceType::LowHealth:SoundToPlay = MaleSoundData.LowHealthBreathVoice;break;
	default:SoundToPlay=nullptr; break;
	}

	if (CharacterGender == ECharacterGender::female)
		switch (VoiceType)
		{
		case EVoiceType::Hurts:SoundToPlay = FemaleSoundData.HurtsVoice; break;
		case EVoiceType::HighHeartBeatRate:SoundToPlay = FemaleSoundData.HurtsVoice; break;
		case EVoiceType::LowEnergy:SoundToPlay = nullptr; break;
		case EVoiceType::FallingFormHeight:SoundToPlay = FemaleSoundData.FallingFromHeightVoice; break;
		case EVoiceType::Death:SoundToPlay = FemaleSoundData.DeathVoice; break;
		case EVoiceType::LowHealth:SoundToPlay = FemaleSoundData.LowHealthBreathVoice; break;
		default:SoundToPlay = nullptr; break;
		}
	return SoundToPlay;
}

void UBFCharacterAudioComponent::SetCharacterGender(ECharacterGender InGender)
{
	this->CharacterGender = InGender;
}

void UBFCharacterAudioComponent::Play(float StartTime /* = 0.f */)
{
	Sound = GetPlaySound(VoiceType);
	Super::Play(StartTime);
}