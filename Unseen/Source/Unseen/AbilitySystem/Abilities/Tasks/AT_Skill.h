// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_Skill.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class UNSEEN_API UAT_Skill : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UAT_Skill();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_Skill* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_SkillHitCheck> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FSkillResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class ATA_SkillHitCheck> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class ATA_SkillHitCheck> SpawnedTargetActor;
};
