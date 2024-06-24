// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TA_SkillHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API ATA_SkillHitCheck : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	ATA_SkillHitCheck();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};
