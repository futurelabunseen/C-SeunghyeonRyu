// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Boss_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API UGA_Boss_Teleport : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Boss_Teleport();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
