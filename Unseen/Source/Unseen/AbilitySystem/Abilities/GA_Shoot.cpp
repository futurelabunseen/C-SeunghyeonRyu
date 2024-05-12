// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Shoot.h"
#include "Character/UnseenCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Weapon/AssaultRifle.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"

UGA_Shoot::UGA_Shoot()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Shoot"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsShooting"));

	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsAiming"));


}

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bIsShooting = true;

	UnseenCharacter->GetWeaponOnHand()->ShootingMontageStart();
	
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	UAnimMontage* ShootMontage = UnseenCharacter->GetShootingMontage();

	if (!AnimInstance->Montage_IsActive(ShootMontage))
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
		const UUnseenCharacterAttributeSet* AttributeSet = ASC->GetSet<UUnseenCharacterAttributeSet>();
		AnimInstance->Montage_Play(ShootMontage, AttributeSet->GetShootRate());
	}
}

void UGA_Shoot::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bIsShooting = false;

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Shoot::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}