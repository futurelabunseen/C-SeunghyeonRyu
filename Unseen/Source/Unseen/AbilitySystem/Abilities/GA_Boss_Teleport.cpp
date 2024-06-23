// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Boss_Teleport.h"
#include "Kismet/GameplayStatics.h"

UGA_Boss_Teleport::UGA_Boss_Teleport()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Boss_Teleport::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Boss Teleport"));

	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FVector PlayerLocation = PlayerActor->GetActorLocation();
	FVector MonsterLocation = ActorInfo->AvatarActor->GetActorLocation();

	FVector DirectionToPlayer = (PlayerLocation - MonsterLocation).GetSafeNormal();
	FVector TeleportLocation = PlayerLocation + DirectionToPlayer * 500.0f;
	TeleportLocation.Z = MonsterLocation.Z;

	FVector Direction = MonsterLocation - PlayerLocation;
	Direction.Z = 0.0f;
	FRotator NewRotation = Direction.Rotation();

	ActorInfo->AvatarActor->SetActorLocation(TeleportLocation);
	ActorInfo->AvatarActor->SetActorRotation(NewRotation);

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
