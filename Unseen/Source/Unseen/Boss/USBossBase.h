// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "USBossBase.generated.h"

UCLASS()
class UNSEEN_API AUSBossBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSBossBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = State)
	uint8 bIsBattleStart : 1;

	UFUNCTION()
	virtual void LimitBattleZone();
};
