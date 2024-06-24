// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Reload.h"
#include "Character/UnseenCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

UGA_Reload::UGA_Reload()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Reload"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsReloading"));

	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Aim"));

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsRolling"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsStepBack"));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsSprinting"));

	static ConstructorHelpers::FObjectFinder<USoundCue> ReloadAudioObject(TEXT("/Script/Engine.SoundCue'/Game/Weapon/ReloadSound_Cue.ReloadSound_Cue'"));
	if (nullptr != ReloadAudioObject.Object)
	{
		ReloadSound = ReloadAudioObject.Object;
	}
}

void UGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	UAnimMontage* ReloadMontage = UnseenCharacter->GetReloadMontage();
	if (!AnimInstance->Montage_IsActive(ReloadMontage))
	{
		AnimInstance->Montage_Play(ReloadMontage, 1.0f);
	}
	AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), ReloadSound);

}

bool UGA_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	
	return UnseenCharacter->IsCanReload();
}

void UGA_Reload::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	UAnimMontage* ReloadMontage = UnseenCharacter->GetReloadMontage();
	if (AnimInstance->Montage_IsActive(ReloadMontage))
	{
		AnimInstance->Montage_Stop(0.0f, ReloadMontage);
	}
	AudioComponent->Stop();
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}


