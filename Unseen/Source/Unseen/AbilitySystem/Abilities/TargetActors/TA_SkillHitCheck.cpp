// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TargetActors/TA_SkillHitCheck.h"
#include "Abilities/GameplayAbility.h"
#include "Boss/USEarthBoss.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ATA_SkillHitCheck::ATA_SkillHitCheck()
{
}

void ATA_SkillHitCheck::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ATA_SkillHitCheck::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle ATA_SkillHitCheck::MakeTargetData() const
{
	AUSEarthBoss* Character = CastChecked<AUSEarthBoss>(SourceActor);
	
	FHitResult OutHitResult;
	//const float AttackRange = 300.0f;
	const float AttackRadius = 500.f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->SkillPos;
	const FVector End = Character->SkillPos;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Start, AttackRadius, 10, DrawColor, false, 3.0f);
	}

#endif

	return DataHandle;
}