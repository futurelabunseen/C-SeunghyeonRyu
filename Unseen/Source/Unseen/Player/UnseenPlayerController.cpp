// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UnseenPlayerController.h"

void AUnseenPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
