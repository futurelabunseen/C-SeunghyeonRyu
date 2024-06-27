// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Boss_PunchHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Abilities/Tasks/AT_PunchHitCheck.h"
#include "AbilitySystem/Abilities/TargetActors/TA_PunchHitCheck.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"
#include "AbilitySystem/Attribute/BossAttributeSet.h"
#include "Character/UnseenCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UGA_Boss_PunchHitCheck::UGA_Boss_PunchHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FObjectFinder<USoundCue> HitSoundObject(TEXT("/Script/Engine.SoundCue'/Game/Sound/hit_Cue.hit_Cue'"));
	if (nullptr != HitSoundObject.Object)
	{
		HitSound = HitSoundObject.Object;
	}
}

void UGA_Boss_PunchHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Punch Hitcheck"));

	UAT_PunchHitCheck* AttackTraceTask = UAT_PunchHitCheck::CreateTask(this, ATA_PunchHitCheck::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UGA_Boss_PunchHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_Boss_PunchHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
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
			const float AttackDamage = SourceAttribute->GetPunchDamage();
			TargetAttribute->SetHp(TargetAttribute->GetHp() - AttackDamage);
			UGameplayStatics::PlaySound2D(this, HitSound);
		}
		
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
