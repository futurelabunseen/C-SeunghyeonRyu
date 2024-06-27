// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNSEEN_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPauseMenu(const FObjectInitializer& ObjectInitializer);

	void InitialUI();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> SRB1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> SRB2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> SRB3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> SRB4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> SRB5;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DVR1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DVR2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DVR3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DVR4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DVR5;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DHR1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DHR2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DHR3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DHR4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UBorder> DHR5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ShootRateCnt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int VerticalCnt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int HorizontalCnt;

	UFUNCTION(BlueprintCallable)
	void FillSRB(int index);
	UFUNCTION(BlueprintCallable)
	void FillDVR(int index);
	UFUNCTION(BlueprintCallable)
	void FillDHR(int index);
};
