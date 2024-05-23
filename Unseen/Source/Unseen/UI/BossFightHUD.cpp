// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossFightHUD.h"

UBossFightHUD::UBossFightHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBossFightHUD::SetHealthBar(float CurrentHp, float MaxHp)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHp / MaxHp);
	}
}
