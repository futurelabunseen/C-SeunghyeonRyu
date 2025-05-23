// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Aim.h"
#include "Character/UnseenCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Aim::UGA_Aim()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Aim"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsAiming"));

	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Sprint"));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsRolling"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsStepBack"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsReloading"));
	
}

void UGA_Aim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bIsAiming = true;
	UnseenCharacter->bUseControllerRotationYaw = true;
	UnseenCharacter->GetCharacterMovement()->MaxWalkSpeed = 180.f;

	UnseenCharacter->AimCameraLerp();
	UE_LOG(LogTemp, Warning, TEXT("aim in"));

}

void UGA_Aim::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bIsAiming = false;;
	//만약 move 중이면 true
	if (!UnseenCharacter->bIsMoving)
	{
		UnseenCharacter->bUseControllerRotationYaw = false;
	}
	UnseenCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;

	UnseenCharacter->AimCameraLerp();
	UE_LOG(LogTemp, Warning, TEXT("aim out"));

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Aim::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UGA_Aim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	//FGameplayTagContainer* Temp = new FGameplayTagContainer();
	//Temp->AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Shoot"));
	//ASC->CancelAbilities(Temp);
	ASC->CancelAbility(ASC->FindAbilitySpecFromInputID(7)->Ability);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
