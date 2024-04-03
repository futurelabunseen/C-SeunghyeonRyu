// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Sprint.h"
#include "Character/UnseenCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Sprint::UGA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsMoving"));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsRolling"));

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsSprinting"));
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());

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

	if (MovementDirection > -0.1)
	{
		return true;
	}

	return false;
}

void UGA_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void UGA_Sprint::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());

	FVector CharacterForward = UnseenCharacter->GetActorForwardVector();
	FVector VelocityDirection = UnseenCharacter->GetVelocity().GetSafeNormal();
	float MovementDirection = FVector::DotProduct(CharacterForward, VelocityDirection);

	if (!UnseenCharacter->GetCharacterMovement()->Velocity.IsNearlyZero() && MovementDirection > -0.1)
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