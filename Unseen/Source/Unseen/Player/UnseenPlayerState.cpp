// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UnseenPlayerState.h"
#include "AbilitySystemComponent.h"


AUnseenPlayerState::AUnseenPlayerState()
{
	// Ability System
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}


UAbilitySystemComponent* AUnseenPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}


