// Fill out your copyright notice in the Description page of Project Settings.


#include "UnseenCharacterHUD.h"


UUnseenCharacterHUD::UUnseenCharacterHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UUnseenCharacterHUD::SetStaminaBar(float CurrentStamina, float MaxStamina)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(CurrentStamina / MaxStamina);
	}
}

void UUnseenCharacterHUD::SetCrossHairPos()
{
	//FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
	//FVector2D ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

	//UE_LOG(LogTemp, Warning, TEXT("%f %f"), ViewportCenter.X, ViewportCenter.Y);


	if (CrossHairLeft)
	{
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairLeft)->SetPosition(FVector2D(- CrossHairLongLength - 4, - CrossHairShortLength / 2));
	}
	if (CrossHairRight)
	{
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairRight)->SetPosition(FVector2D(4, - CrossHairShortLength / 2));
	}
	if (CrossHairTop)
	{
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairTop)->SetPosition(FVector2D(- CrossHairShortLength / 2, - CrossHairLongLength / 2 - 4));
	}
	if (CrossHairBottom)
	{
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairBottom)->SetPosition(FVector2D(- CrossHairShortLength / 2, 4));
	}
}

void UUnseenCharacterHUD::SetHorizontalCrossHairPos(float HorizontalRecoil)
{
	if (CrossHairLeft)
	{
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairLeft)->SetPosition(FVector2D(-CrossHairLongLength - 4 - HorizontalRecoil, -CrossHairShortLength / 2));
	}
	if (CrossHairRight)
	{
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairRight)->SetPosition(FVector2D(4 + HorizontalRecoil, -CrossHairShortLength / 2));
	}
}

void UUnseenCharacterHUD::NativeOnInitialized()
{
	if (StaminaBar)
	{
		StaminaBar->BarFillType = EProgressBarFillType::LeftToRight;
		StaminaBar->BarFillStyle = EProgressBarFillStyle::Mask;
		StaminaBar->SetFillColorAndOpacity(FLinearColor{ 0, 0.5f, 1.0f, 1.0f });
	}

	if (HpBar)
	{
		HpBar->BarFillType = EProgressBarFillType::LeftToRight;
		HpBar->BarFillStyle = EProgressBarFillStyle::Mask;
		HpBar->SetFillColorAndOpacity(FLinearColor{ 1.0f, 0.0f, 0.0f, 1.0f });
	}

	if (CrossHairLeft)
	{
		CrossHairLeft->SetColorAndOpacity(FLinearColor{ 1.0f, 0, 0, 1.0f });
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairLeft)->SetSize(FVector2D(CrossHairLongLength, CrossHairShortLength));
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairLeft)->SetAnchors(0.5f);

	}
	if (CrossHairRight)
	{
		CrossHairRight->SetColorAndOpacity(FLinearColor{ 1.0f, 0, 0, 1.0f });
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairRight)->SetSize(FVector2D(CrossHairLongLength, CrossHairShortLength));
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairRight)->SetAnchors(0.5f);
	}
	if (CrossHairTop)
	{
		CrossHairTop->SetColorAndOpacity(FLinearColor{ 1.0f, 0, 0, 1.0f });
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairTop)->SetSize(FVector2D(CrossHairShortLength, CrossHairLongLength / 2));
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairTop)->SetAnchors(0.5f);
	}
	if (CrossHairBottom)
	{
		CrossHairBottom->SetColorAndOpacity(FLinearColor{ 1.0f, 0, 0, 1.0f });
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairBottom)->SetSize(FVector2D(CrossHairShortLength, CrossHairLongLength / 2));
		UWidgetLayoutLibrary::SlotAsCanvasSlot(CrossHairBottom)->SetAnchors(0.5f);
	}

	SetCrossHairPos();
}
