// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
*  define world surface types
*/
#define SurfaceType_Default                                        SurfaceType_Default
#define SurfaceType_Dirt                                             SurfaceType1
#define SurfaceType_Concret                                       SurfaceType2
#define SurfaceType_ThinMetal                                     SurfaceType3
#define SurfaceType_ThickMetal                                   SurfaceType4
#define SurfaceType_Grass                                          SurfaceType5
#define SurfaceType_Glass                                          SurfaceType6
#define SurfaceType_Water                                          SurfaceType7
#define SurfaceType_Wood                                          SurfaceType8
#define SurfaceType_Alsphat                                        SurfaceType9
#define SurfaceType_Ruber                                          SurfaceType10
#define SurfaceType_Body                                           SurfaceType11
#define SurfaceType_Gravel                                          SurfaceType12
#define SurfaceType_Mud                                             SurfaceType13
#define SurfaceType_Can                                             SurfaceType14
#define SurfaceType_Fabric                                          SurfaceType15
#define SurfaceType_Plastic                                         SurfaceType16
#define SurfaceType_Carpet                                          SurfaceType17
#define SurfaceTYpe_TreeLog                                       SurfaceType18
#define SurfaceType_TreeLeaves                                   SurfaceType19
#define SurfaceType_Armor                                           SurfaceType20


/**  global define the character's voice type */
UENUM(BlueprintType)
enum class EVoiceType :uint8
{
	Hurts,
	HighHeartBeatRate,
	LowEnergy,
	FallingFormHeight,
	Death,
	LowHealth
};

/** ammo type, weapon should specify ammo type and different ammo type are not compatible */
UENUM(BlueprintType)
enum class EProjectileType :uint8 {
	SMALL                                           UMETA(DisplayName = "5.56mm"),
	MEADEIUM                                   UMETA(DisplayName = "7.56mm"),
	LARGE                                          UMETA(DisplayName = "9.0mm"),
	ShotGun                                        UMETA(DisplayName = "ShotGun")
};

/** specify a character' gender,can be assigned via blueprint */
UENUM(BlueprintType)
enum class ECharacterGender :uint8
{
	male,
	female
};

/**2 Type of Team roles*/
UENUM(BlueprintType)
enum class ETeam :uint8
{
	CT         UMETA(DisplayName = "Counter Terrolist"),
	T           UMETA(DisplayName = "Terrolist")
};


