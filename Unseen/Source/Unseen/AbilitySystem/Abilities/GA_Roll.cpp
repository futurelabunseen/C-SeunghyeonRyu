// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Roll.h"
#include "Character/UnseenCharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Roll::UGA_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsMoving"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsRolling"));

}

void UGA_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bUseControllerRotationYaw = false;

	UnseenCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	UnseenCharacter->SetActorRotation(FRotationMatrix::MakeFromX(UnseenCharacter->GetCharacterMovement()->GetLastInputVector()).Rotator());

	UAbilityTask_PlayMontageAndWait* PlayRollTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayRoll"), UnseenCharacter->GetRollMontage());

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

	return true;
}

void UGA_Roll::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
}

void UGA_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	UnseenCharacter->bUseControllerRotationYaw = true;
}

void UGA_Roll::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
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
