// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Roll.h"
#include "Character/UnseenCharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"


UGA_Roll::UGA_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Roll"));

	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsMoving"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsRolling"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Move"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Aim"));


}

void UGA_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->StopRegenStamina();
	UnseenCharacter->StartRegenStaminaWithDelay(4.5f);

	// Use Stamina
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(UseStaminaEffect);
	if (EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}

	// Roll Anim
	
	UnseenCharacter->SetActorRotation(UnseenCharacter->CalculateRollDirection().Rotation().Quaternion());
	//UnseenCharacter->SetActorRotation(FRotationMatrix::MakeFromX(UnseenCharacter->CalculateRollDirection()).Rotator());
	UnseenCharacter->bIsRollStepBackActive = true;
	UnseenCharacter->RollStepBackCameraLerp();
	UAbilityTask_PlayMontageAndWait* PlayRollTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayRoll"), UnseenCharacter->GetRollMontage());

	UnseenCharacter->GetRollMovementTimeline()->PlayFromStart();

	PlayRollTask->OnCompleted.AddDynamic(this, &UGA_Roll::OnCompleteCallback);
	PlayRollTask->OnInterrupted.AddDynamic(this, &UGA_Roll::OnInterruptedCallback);
	PlayRollTask->ReadyForActivation();
	
}

bool UGA_Roll::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// 스테미나 검사
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	const UUnseenCharacterAttributeSet* AttributeSet = ASC->GetSet<UUnseenCharacterAttributeSet>();
	if (AttributeSet->GetStamina() < -AttributeSet->GetRollStaminaCost())
	{
		return false;
	}

	return true;
}

void UGA_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bIsRollStepBackActive = false;
	UnseenCharacter->RollStepBackCameraLerp();
	UnseenCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}

void UGA_Roll::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_Roll::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}