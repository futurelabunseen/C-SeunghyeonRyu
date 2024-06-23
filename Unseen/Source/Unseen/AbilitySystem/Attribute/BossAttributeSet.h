// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BossAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class UNSEEN_API UBossAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UBossAttributeSet();

	ATTRIBUTE_ACCESSORS(UBossAttributeSet, PunchDamage);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BossInfo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PunchDamage;
};
