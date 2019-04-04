// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "BioFeild.h"
#include "BFCharacterAudioComponent.generated.h"


class ABFBaseCharacter;
class ABFVoice;
class UBFSkeletalMeshComponent;
class USoundCue;

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FCharacterSoundData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		USoundCue* ScreamVoice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		USoundCue* FallingFromHeightVoice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		USoundCue* DeathVoice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		USoundCue* HurtsVoice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		USoundCue* LowHealthBreathVoice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		USoundCue* NormalBreathSound;
};
UCLASS()
class BIOFEILD_API UBFCharacterAudioComponent : public UAudioComponent
{
	GENERATED_UCLASS_BODY()
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		FCharacterSoundData MaleSoundData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		FCharacterSoundData FemaleSoundData;

private:

	UPROPERTY()
	ABFBaseCharacter* CharacterOwner;

	UPROPERTY()
		uint8 bIsZombie : 1;

	/** cached character skeletal mesh components */
	UBFSkeletalMeshComponent* CharacterMesh;

	ECharacterGender CharacterGender;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void SetOwner(ABFBaseCharacter* NewOwner);

	virtual ABFBaseCharacter* GetCharacterOwner()const { return CharacterOwner; }

	virtual USoundCue* GetPlaySound(EVoiceType VoiceType);

	virtual void SetCharacterGender(ECharacterGender InGender);

	virtual void PlayVoice(EVoiceType VoiceType);
};
