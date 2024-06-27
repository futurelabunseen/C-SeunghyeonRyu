// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNSEEN_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameOver(const FObjectInitializer& ObjectInitializer);
};
