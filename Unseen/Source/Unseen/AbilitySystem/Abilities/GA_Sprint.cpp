// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Sprint.h"
#include "Character/UnseenCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"

UGA_Sprint::UGA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Sprint"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsSprinting"));

	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsMoving"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsRolling"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsAiming"));

	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Reload"));
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bIsSprinting = true;
	UnseenCharacter->StopRegenStamina();
	
	// Use Stamina
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(UseStaminaEffect);
	if (EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}

	UnseenCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

bool UGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());

	FVector CharacterForward = UnseenCharacter->GetActorForwardVector();
	FVector VelocityDirection = UnseenCharacter->GetVelocity().GetSafeNormal();
	float MovementDirection = FVector::DotProduct(CharacterForward, VelocityDirection);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	const UUnseenCharacterAttributeSet* AttributeSet = ASC->GetSet<UUnseenCharacterAttributeSet>();

	// minimum stamina >= 5
	if (AttributeSet->GetStamina() >= 5 && MovementDirection > -0.1)
	{
		return true;
	}

	return false;
}

void UGA_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());

	UnseenCharacter->StartRegenStaminaWithDelay(2.0f);
	UnseenCharacter->bIsSprinting = false;

	UnseenCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Sprint::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());

	FVector CharacterForward = UnseenCharacter->GetActorForwardVector();
	FVector VelocityDirection = UnseenCharacter->GetVelocity().GetSafeNormal();
	float MovementDirection = FVector::DotProduct(CharacterForward, VelocityDirection);

	// 스테미나 검사
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	const UUnseenCharacterAttributeSet* AttributeSet = ASC->GetSet<UUnseenCharacterAttributeSet>();
	bool bIsEnoughStamina = !(AttributeSet->GetStamina() < -AttributeSet->GetSprintStaminaCost());

	if (bIsEnoughStamina && !UnseenCharacter->GetCharacterMovement()->Velocity.IsNearlyZero() && MovementDirection > -0.1)
	{
		Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	}
	else
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}