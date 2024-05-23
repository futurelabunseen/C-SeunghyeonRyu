// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/USBossBase.h"
#include "USEarthBoss.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API AUSEarthBoss : public AUSBossBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AUSEarthBoss();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void LimitBattleZone() override;



};
