// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/AT_Skill.h"
#include "AbilitySystem/Abilities/TargetActors/TA_SkillHitCheck.h"
#include "AbilitySystemComponent.h"

UAT_Skill::UAT_Skill()
{
}

UAT_Skill* UAT_Skill::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_SkillHitCheck> TargetActorClass)
{
	UAT_Skill* NewTask = NewAbilityTask<UAT_Skill>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UAT_Skill::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UAT_Skill::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UAT_Skill::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ATA_SkillHitCheck>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAT_Skill::OnTargetDataReadyCallback);
	}
}

void UAT_Skill::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UAT_Skill::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
