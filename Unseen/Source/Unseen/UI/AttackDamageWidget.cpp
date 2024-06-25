// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AttackDamageWidget.h"
#include "Components/TextBlock.h"

void UAttackDamageWidget::SetDamage(float Damage, FLinearColor Color)
{
	uint32 Score = static_cast<uint8>(Damage);
	FString DamageTxt = FString::Printf(TEXT("%d"), Score);
	TxtAttackDamage->SetText(FText::FromString(DamageTxt));
	TxtAttackDamage->SetColorAndOpacity(Color);
}
