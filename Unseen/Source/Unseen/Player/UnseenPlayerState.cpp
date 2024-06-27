// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UnseenPlayerState.h"
#include "AbilitySystemComponent.h"


AUnseenPlayerState::AUnseenPlayerState()
{
	// Ability System
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UUnseenCharacterAttributeSet>(TEXT("AttributeSet"));
}


UAbilitySystemComponent* AUnseenPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
