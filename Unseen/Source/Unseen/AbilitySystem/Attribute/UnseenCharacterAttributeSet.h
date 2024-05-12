// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UnseenCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class UNSEEN_API UUnseenCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UUnseenCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UUnseenCharacterAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UUnseenCharacterAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UUnseenCharacterAttributeSet, RollStaminaCost);
	ATTRIBUTE_ACCESSORS(UUnseenCharacterAttributeSet, StepBackStaminaCost);
	ATTRIBUTE_ACCESSORS(UUnseenCharacterAttributeSet, SprintStaminaCost);
	ATTRIBUTE_ACCESSORS(UUnseenCharacterAttributeSet, ShootRate);


	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="CharacterInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData RollStaminaCost;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StepBackStaminaCost;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SprintStaminaCost;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShootRate;


	friend class UGE_RollStamina; // Áö±Ý ¾È ¾¸
	friend class UGE_StepBackStamina; // Áö±Ý ¾È ¾¸
};
