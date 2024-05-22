// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "US_ProjectileBase.generated.h"

UCLASS()
class UNSEEN_API AUS_ProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUS_ProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetProjectileActive(bool IsActive);

	UFUNCTION()
	void PushPoolSelf();

	UPROPERTY()
	TObjectPtr<class UProjectilePoolComponent_Base> ProjectilePool;

	UPROPERTY()
	FTimerHandle TimerHandle_LifeSpanToPoolExpired;

	UFUNCTION()
	void SetLifeSpanToPool();

	UPROPERTY()
	float LifeSpanTime;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;
};
