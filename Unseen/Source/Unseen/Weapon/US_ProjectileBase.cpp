// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/US_ProjectileBase.h"

// Sets default values
AUS_ProjectileBase::AUS_ProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUS_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUS_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

