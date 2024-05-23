// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "BossFightHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API UBossFightHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UBossFightHUD(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetHealthBar(float CurrentHp, float MaxHp);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;
};
