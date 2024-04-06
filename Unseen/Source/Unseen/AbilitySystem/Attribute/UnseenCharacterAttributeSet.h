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

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="CharacterInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;


	friend class UGE_RollStamina;
};
