// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BioFeild.h"
#include "BFVoiceComponent.generated.h"


class ABFBaseCharacter;
class ABFVoice;
class UBFSkeletalMeshComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIOFEILD_API UBFVoiceComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		TSubclassOf<ABFVoice> MaleSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Voice|Male")
		TSubclassOf<ABFVoice> FeMaleSoundClass;


private:

	ABFBaseCharacter* CharacterOwner;

	/** cached character skeletal mesh components */
	UBFSkeletalMeshComponent* CharacterMesh;

	ECharacterGender CharacterGender;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void SetOwner(ABFBaseCharacter* NewOwner);

	virtual ABFBaseCharacter* GetCharacterOwner()const { return CharacterOwner; }

	/** simulate character speak such as scream when get hurt */
	virtual void Speak(EVoiceType VoiceType);

	virtual void SetCharacterGender(ECharacterGender InGender);
};
