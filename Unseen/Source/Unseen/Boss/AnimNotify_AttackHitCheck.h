// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API UAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_AttackHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;
};
