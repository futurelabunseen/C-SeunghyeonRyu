// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Boss_Skill_02.h"

UGA_Boss_Skill_02::UGA_Boss_Skill_02()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Boss_Skill_02::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Skill02"));
}

void UGA_Boss_Skill_02::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
}
