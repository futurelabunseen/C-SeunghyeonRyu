// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Boss_Teleport.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/USEarthBoss.h"

UGA_Boss_Teleport::UGA_Boss_Teleport()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Boss_Teleport::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Boss Teleport"));

	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AUSEarthBoss* MonsterActor = CastChecked<AUSEarthBoss>(ActorInfo->AvatarActor);

	FVector MonsterSpawnedLocation = MonsterActor->SpawnedPos;
	FVector PlayerLocation = PlayerActor->GetActorLocation();
	//FVector MonsterLocation = ActorInfo->AvatarActor->GetActorLocation();
	
	FVector PlayerForward = PlayerActor->GetActorForwardVector();
	PlayerForward.Z = 0.0f;
	PlayerForward = PlayerForward.GetSafeNormal();

	FVector TeleportLocation = PlayerLocation + PlayerForward * -500.0f;
	TeleportLocation.Z = MonsterSpawnedLocation.Z;

	float Distance1 = FVector::Dist(TeleportLocation, MonsterSpawnedLocation);
	if (Distance1 > 3800.0f)
	{
		FVector SpawnPosToPlayer = (PlayerLocation - MonsterSpawnedLocation);
		SpawnPosToPlayer.Z = 0.0f;
		SpawnPosToPlayer = SpawnPosToPlayer.GetSafeNormal();
	
		TeleportLocation = PlayerLocation + SpawnPosToPlayer * -500.0f;
	}
	
	FVector TeleportPosToPlayer = PlayerLocation - TeleportLocation;
	TeleportPosToPlayer.Z = 0;
	TeleportPosToPlayer = TeleportPosToPlayer.GetSafeNormal();

	FRotator NewRotation = TeleportPosToPlayer.Rotation();
	
	ActorInfo->AvatarActor->SetActorLocation(TeleportLocation);
	ActorInfo->AvatarActor->SetActorRotation(NewRotation);

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
