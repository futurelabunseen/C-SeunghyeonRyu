// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/US_ProjectileBase.h"
#include "Weapon/ProjectilePoolComponent_Base.h"

// Sets default values
AUS_ProjectileBase::AUS_ProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectilePool = nullptr;
	LifeSpanTime = 0.0f;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
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

void AUS_ProjectileBase::SetProjectileActive(bool IsActive)
{
	SetActorHiddenInGame(!IsActive);
	SetActorEnableCollision(IsActive);
	SetActorTickEnabled(IsActive);

	if (!IsActive)
	{
		ProjectileMovementComp->Velocity = FVector::ZeroVector;
	}
	else
	{
		ProjectileMovementComp->Velocity = GetActorForwardVector() * 7000.0f;
	}
}

void AUS_ProjectileBase::PushPoolSelf()
{
	if (ProjectilePool == nullptr)
	{
		return;
	}
	ProjectilePool->PushProjectileInPool(this);
	SetProjectileActive(false);
	UE_LOG(LogTemp, Warning, TEXT("Pool Self : %d"), ProjectilePool->Pool.Num());
}

void AUS_ProjectileBase::SetLifeSpanToPool()
{
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(TimerHandle_LifeSpanToPoolExpired, this, &AUS_ProjectileBase::PushPoolSelf, LifeSpanTime);
	}
}

