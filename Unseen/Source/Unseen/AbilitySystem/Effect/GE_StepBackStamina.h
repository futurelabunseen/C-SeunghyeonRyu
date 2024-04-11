// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_StepBackStamina.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API UGE_StepBackStamina : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UGE_StepBackStamina();

private:
	float cost = 30.0f;
};
