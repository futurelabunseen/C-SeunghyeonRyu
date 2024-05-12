// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API UAnimNotify_Shoot : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_Shoot();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
