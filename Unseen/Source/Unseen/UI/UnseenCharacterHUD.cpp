// Fill out your copyright notice in the Description page of Project Settings.


#include "UnseenCharacterHUD.h"
#include "Components/ProgressBar.h"

UUnseenCharacterHUD::UUnseenCharacterHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (StaminaBar)
	{
		StaminaBar->BarFillType = EProgressBarFillType::LeftToRight;
		StaminaBar->BarFillStyle = EProgressBarFillStyle::Mask;
		StaminaBar->SetFillColorAndOpacity(FLinearColor{ 0, 0.5f, 1.0f, 1.0f });
	}
}

void UUnseenCharacterHUD::SetStaminaBar(float CurrentStamina, float MaxStamina)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(CurrentStamina / MaxStamina);
	}
}
