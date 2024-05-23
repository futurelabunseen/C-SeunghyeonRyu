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

bool UGA_Shoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UAbilitySystemComponent* ASC = UnseenCharacter->GetAbilitySystemComponent();

	if (UnseenCharacter->GetWeaponOnHand()->CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Ammo"));

		//0발에 사격하면 재장전하게 하려고 했는데 차피 aim 중이라 안됨.
		//ASC->CancelAbility(ASC->FindAbilitySpecFromInputID(6)->Ability);
		ASC->TryActivateAbility(ASC->FindAbilitySpecFromInputID(8)->Handle);

		return false;
	}

	return true;
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