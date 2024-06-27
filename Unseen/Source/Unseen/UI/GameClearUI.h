// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearUI.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNSEEN_API UGameClearUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameClearUI(const FObjectInitializer& ObjectInitializer);
};
