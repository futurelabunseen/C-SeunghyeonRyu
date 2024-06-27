// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu.h"

UPauseMenu::UPauseMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ShootRateCnt = 0;
	VerticalCnt = 0;
	HorizontalCnt = 0;
}

void UPauseMenu::InitialUI()
{
	for (int i = 0; i < ShootRateCnt; i++)
	{
		FillSRB(i);
	}
	for (int i = 0; i < VerticalCnt; i++)
	{
		FillDVR(i);
	}
	for (int i = 0; i < HorizontalCnt; i++)
	{
		FillDHR(i);
	}
}

void UPauseMenu::FillSRB(int index)
{
	if (index == 0)
	{
		SRB1->SetBrushColor(FLinearColor::Yellow);
	}
	else if (index == 1)
	{
		SRB2->SetBrushColor(FLinearColor::Yellow);
	}
	else if (index == 2)
	{
		SRB3->SetBrushColor(FLinearColor::Yellow);
	}
	else if (index == 3)
	{
		SRB4->SetBrushColor(FLinearColor::Yellow);
	}
	else if (index == 4)
	{
		SRB5->SetBrushColor(FLinearColor::Yellow);
	}
}

void UPauseMenu::FillDVR(int index)
{
	if (index == 0)
	{
		DVR1->SetBrushColor(FLinearColor::Green);
	}
	else if (index == 1)
	{
		DVR2->SetBrushColor(FLinearColor::Green);
	}
	else if (index == 2)
	{
		DVR3->SetBrushColor(FLinearColor::Green);
	}
	else if (index == 3)
	{
		DVR4->SetBrushColor(FLinearColor::Green);
	}
	else if (index == 4)
	{
		DVR5->SetBrushColor(FLinearColor::Green);
	}
}

void UPauseMenu::FillDHR(int index)
{
	if (index == 0)
	{
		DHR1->SetBrushColor(FLinearColor::Blue);
	}
	else if (index == 1)
	{
		DHR2->SetBrushColor(FLinearColor::Blue);
	}
	else if (index == 2)
	{
		DHR3->SetBrushColor(FLinearColor::Blue);
	}
	else if (index == 3)
	{
		DHR4->SetBrushColor(FLinearColor::Blue);
	}
	else if (index == 4)
	{
		DHR5->SetBrushColor(FLinearColor::Blue);
	}
}
