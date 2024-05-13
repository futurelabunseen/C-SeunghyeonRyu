// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnseenWeaponBase.generated.h"

UENUM()
enum class EWeaponPart : uint8 {

	MainBody = 0,
	BulletSleeve = 1,
	Magazine = 2,
};


UCLASS()
class UNSEEN_API AUnseenWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnseenWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void ShootingMontageStart();
	UFUNCTION()
	virtual void ShootingStop();

	UFUNCTION()
	virtual void ShootWeapon();

	UFUNCTION()
	FORCEINLINE bool IsAmmoFull() const { return CurrentAmmo == MaxAmmo; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	float ShootRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	int MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
	int CurrentAmmo;
};
