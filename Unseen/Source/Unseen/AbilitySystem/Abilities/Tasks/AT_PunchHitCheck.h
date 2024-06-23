// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_PunchHitCheck.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
  */
UCLASS()
class UNSEEN_API UAT_PunchHitCheck : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAT_PunchHitCheck();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_PunchHitCheck* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_PunchHitCheck> TargetActorClass);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class ATA_PunchHitCheck> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class ATA_PunchHitCheck> SpawnedTargetActor;
};
