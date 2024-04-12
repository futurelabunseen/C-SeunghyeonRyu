// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"

UUnseenCharacterAttributeSet::UUnseenCharacterAttributeSet() :
	Stamina(100.0f),
	MaxStamina(100.0f),
	RollStaminaCost(-30.0f),
	StepBackStaminaCost(-30.0f),
	SprintStaminaCost(-1.0f)
{
	InitStamina(GetMaxStamina());
}

void UUnseenCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetStaminaAttribute())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Before Clamp : %f"), NewValue);
		
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
		
		//UE_LOG(LogTemp, Warning, TEXT("After Clamp : %f"), NewValue);

		// 에디터 디버깅 창에는 100으로 나온다.
		// 그러나 어째서?
		// Base Value와 Current Value가 나뉘어있다.
		// 이거 관련 문제인듯
		// Attribute set에서는 CurrentValue만 바꾸는듯.
	}
	Super::PreAttributeChange(Attribute, NewValue);
}

void UUnseenCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetStaminaAttribute())
	{
		GetStaminaAttribute().GetGameplayAttributeData(this)->SetBaseValue(NewValue);
		//GetStaminaAttribute().GetGameplayAttributeData(this)->SetCurrentValue(NewValue);
		//UE_LOG(LogTemp, Warning, TEXT("Get Base Value : %f"), GetStaminaAttribute().GetGameplayAttributeData(this)->GetBaseValue());
		//UE_LOG(LogTemp, Warning, TEXT("Get Current Value : %f"), GetStaminaAttribute().GetGameplayAttributeData(this)->GetCurrentValue());
	}
}
