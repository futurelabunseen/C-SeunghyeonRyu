// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttackDamageWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNSEEN_API UAttackDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetDamage(float Damage, FLinearColor Color);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtAttackDamage;
};
