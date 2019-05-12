// Fill out your copyright notice in the Description page of Project Settings.

#include "BFUserWidgetBase.h"


UBFUserWidgetBase::UBFUserWidgetBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
   
}

void UBFUserWidgetBase::ShowWidget()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UBFUserWidgetBase::HideWidget()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

