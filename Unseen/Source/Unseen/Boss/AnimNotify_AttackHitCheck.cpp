// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/AnimNotify_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_AttackHitCheck::UAnimNotify_AttackHitCheck()
{
}

FString UAnimNotify_AttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
