// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"

UUnseenCharacterAttributeSet::UUnseenCharacterAttributeSet() :
	Stamina(100.0f),
	MaxStamina(100.0f),
	RollStaminaCost(-30.0f),
	StepBackStaminaCost(-30.0f),
	SprintStaminaCost(-1.0f),
	ShootRate(1.0f)
{
	InitStamina(GetMaxStamina());
}

void UUnseenCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	Super::PreAttributeChange(Attribute, NewValue);
}

void UUnseenCharacterAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UUnseenCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	/*if (Attribute == GetStaminaAttribute())
	{
		GetStaminaAttribute().GetGameplayAttributeData(this)->SetBaseValue(NewValue);
	}*/
}
