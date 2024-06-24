// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Tasks/AT_PunchHitCheck.h"
#include "AbilitySystem/Abilities/TargetActors/TA_PunchHitCheck.h"
#include "AbilitySystemComponent.h"

UAT_PunchHitCheck::UAT_PunchHitCheck()
{
}

UAT_PunchHitCheck* UAT_PunchHitCheck::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_PunchHitCheck> TargetActorClass)
{
	UAT_PunchHitCheck* NewTask = NewAbilityTask<UAT_PunchHitCheck>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UAT_PunchHitCheck::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();
	
	SetWaitingOnAvatar();
}

void UAT_PunchHitCheck::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UAT_PunchHitCheck::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ATA_PunchHitCheck>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		//SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAT_PunchHitCheck::OnTargetDataReadyCallback);
	}
}

void UAT_PunchHitCheck::FinalizeTargetActor()
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

void UAT_PunchHitCheck::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
