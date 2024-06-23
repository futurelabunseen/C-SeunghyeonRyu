// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Boss_Skill_01.h"

UGA_Boss_Skill_01::UGA_Boss_Skill_01()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Boss_Skill_01::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Skill01"));
}

void UGA_Boss_Skill_01::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
}
