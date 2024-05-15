// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> CrossHairLeft;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> CrossHairRight;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> CrossHairTop;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> CrossHairBottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CrossHair)
	int CrossHairLongLength = 18;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CrossHair)
	int CrossHairShortLength = 4;

	UFUNCTION()
	void SetCrossHairPos();
	void SetHorizontalCrossHairPos(float HorizontalRecoil);

protected:
	virtual void NativeOnInitialized();


};
