// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Boss_Skill_01.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Abilities/Tasks/AT_Skill.h"
#include "AbilitySystem/Abilities/TargetActors/TA_SkillHitCheck.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"
#include "AbilitySystem/Attribute/BossAttributeSet.h"
#include "Character/UnseenCharacterPlayer.h"

UGA_Boss_Skill_01::UGA_Boss_Skill_01()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Boss_Skill_01::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Skill01"));

	UAT_Skill* AttackTraceTask = UAT_Skill::CreateTask(this, ATA_SkillHitCheck::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UGA_Boss_Skill_01::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_Boss_Skill_01::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogTemp, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (!SourceASC || !TargetASC)
		{
			UE_LOG(LogTemp, Error, TEXT("ASC not found!"));
			return;
		}

		const UBossAttributeSet* SourceAttribute = SourceASC->GetSet<UBossAttributeSet>();
		UUnseenCharacterAttributeSet* TargetAttribute = const_cast<UUnseenCharacterAttributeSet*>(TargetASC->GetSet<UUnseenCharacterAttributeSet>());
		if (!SourceAttribute || !TargetAttribute)
		{
			UE_LOG(LogTemp, Error, TEXT("ASC not found!"));
			return;
		}
		if (!CastChecked<AUnseenCharacterPlayer>(HitResult.GetActor())->bIsRollStepBackActive)
		{
			const float SkillDamage = SourceAttribute->GetSkillDamage();
			TargetAttribute->SetHp(TargetAttribute->GetHp() - SkillDamage);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
