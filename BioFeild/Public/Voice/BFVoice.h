// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BioFeild.h"
#include "BFVoice.generated.h"

class USoundCue;
class ABFBaseCharacter;

UCLASS()
class BIOFEILD_API ABFVoice : public AActor
{
	GENERATED_UCLASS_BODY()

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

protected:

	/** voice type that determine which voice  */
	EVoiceType VoiceType;

	/** Character that speak this voice */
	ABFBaseCharacter*  Character;

	virtual void PostInitializeComponents()override;

public:

	virtual USoundCue* SelectSoundByVoiceType();

	virtual void  SetVoiceType(EVoiceType VoiceType);

	virtual void SetCharacter(ABFBaseCharacter*  Character);
};
