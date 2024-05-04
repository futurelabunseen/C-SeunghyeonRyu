// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnseenCharacterHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNSEEN_API UUnseenCharacterHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUnseenCharacterHUD(const FObjectInitializer& ObjectInitializer);

	void SetStaminaBar(float CurrentStamina, float MaxStamina);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UProgressBar> StaminaBar;

};
