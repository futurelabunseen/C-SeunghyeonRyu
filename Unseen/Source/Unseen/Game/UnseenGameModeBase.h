// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "UnseenGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawnDelegate);
/**
 * 
 */
UCLASS()
class UNSEEN_API AUnseenGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnseenGameModeBase();

	UFUNCTION(BlueprintCallable)
	void RespawnPlayer();
	
	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UPROPERTY()
	TObjectPtr<class APlayerController> PlayerController;

	UFUNCTION()
	void SpawnBoss();
	UPROPERTY()
	TSubclassOf<AActor> BossMonsterClass;

	UPROPERTY(BlueprintAssignable)
	FOnRespawnDelegate OnRespawn;
};
