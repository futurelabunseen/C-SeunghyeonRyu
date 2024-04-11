// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/GE_StepBackStamina.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"

UGE_StepBackStamina::UGE_StepBackStamina()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo StaminaModifier;
	StaminaModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UUnseenCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UUnseenCharacterAttributeSet, Stamina)));
	StaminaModifier.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat UseAmount(-cost);
	FGameplayEffectModifierMagnitude ModMagnitude(UseAmount);

	StaminaModifier.ModifierMagnitude = ModMagnitude;
	Modifiers.Add(StaminaModifier);
}
