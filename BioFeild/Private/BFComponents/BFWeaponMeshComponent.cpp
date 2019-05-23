// Fill out your copyright notice in the Description page of Project Settings.

#include "BFWeaponMeshComponent.h"

UBFWeaponMeshComponent::UBFWeaponMeshComponent(const FObjectInitializer& ObjectInitailizer) :Super(ObjectInitailizer)
{
	MuzzleFlashSocket = FName(TEXT("MuzzleFlashSocket"));
	ShellEjectSocket = FName(TEXT("ShellEjectSocket"));
	ScopeSocket = FName(TEXT("ScopeSocket"));
	ScopeHolderSocket = FName(TEXT("ScopeHolderSocket"));
}


