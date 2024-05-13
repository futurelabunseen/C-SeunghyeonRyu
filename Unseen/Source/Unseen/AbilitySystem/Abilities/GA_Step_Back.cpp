// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Step_Back.h"
#include "Character/UnseenCharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"

UGA_Step_Back::UGA_Step_Back()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.StepBack"));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsMoving"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsRolling"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsStepBack"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Aim"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Reload"));
}

void UGA_Step_Back::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());

	UnseenCharacter->StopRegenStamina();
	UnseenCharacter->StartRegenStaminaWithDelay(4.07f);

	// Use Stamina
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(UseStaminaEffect);
	if (EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}

	
	FRotator ControllerRotator(0.f, UnseenCharacter->GetControlRotation().Yaw, 0.f);
	UnseenCharacter->SetActorRotation(ControllerRotator);
	
	///UnseenCharacter->bUseControllerRotationYaw = false;

	UAbilityTask_PlayMontageAndWait* PlayStepBackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayStepBack"), UnseenCharacter->GetStepBackMontage());
	
	UnseenCharacter->bIsRollStepBackActive = true;
	UnseenCharacter->RollStepBackCameraLerp();

	UnseenCharacter->GetStepBackMovementTimeline()->PlayFromStart();

	PlayStepBackTask->OnCompleted.AddDynamic(this, &UGA_Step_Back::OnCompleteCallback);
	PlayStepBackTask->OnInterrupted.AddDynamic(this, &UGA_Step_Back::OnInterruptedCallback);
	PlayStepBackTask->ReadyForActivation();

}

bool UGA_Step_Back::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// 스테미나 검사
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	const UUnseenCharacterAttributeSet* AttributeSet = ASC->GetSet<UUnseenCharacterAttributeSet>();
	if (AttributeSet->GetStamina() < -AttributeSet->GetStepBackStaminaCost())
	{
		return false;
	}

	return true;
}

void UGA_Step_Back::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bIsRollStepBackActive = false;
	UnseenCharacter->RollStepBackCameraLerp();
	UnseenCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGA_Step_Back::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_Step_Back::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
