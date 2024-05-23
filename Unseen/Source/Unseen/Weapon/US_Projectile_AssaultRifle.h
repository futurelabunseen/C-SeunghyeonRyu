// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/US_ProjectileBase.h"
#include "US_Projectile_AssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API AUS_Projectile_AssaultRifle : public AUS_ProjectileBase
{
	GENERATED_BODY()

public:
	AUS_Projectile_AssaultRifle();
	
protected:
	virtual void BeginPlay() override;

public:
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
