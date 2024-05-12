// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/UnseenWeaponBase.h"

// Sets default values
AUnseenWeaponBase::AUnseenWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShootRate = 1.0f;
}

// Called when the game starts or when spawned
void AUnseenWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnseenWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnseenWeaponBase::ShootingMontageStart()
{
	
}

void AUnseenWeaponBase::ShootingStop()
{
	
}

void AUnseenWeaponBase::ShootWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("ShootWeapon"));
}

