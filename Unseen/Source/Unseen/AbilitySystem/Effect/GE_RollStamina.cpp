// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effect/GE_RollStamina.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"

UGE_RollStamina::UGE_RollStamina()
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
