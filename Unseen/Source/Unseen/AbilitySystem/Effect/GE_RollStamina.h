// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_RollStamina.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API UGE_RollStamina : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UGE_RollStamina();

private:
	float cost = 30.0f;
};
