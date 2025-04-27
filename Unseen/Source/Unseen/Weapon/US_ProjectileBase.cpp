// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/US_ProjectileBase.h"
#include "Weapon/ProjectilePoolComponent_Base.h"


AUS_ProjectileBase::AUS_ProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectilePool = nullptr;
	LifeSpanTime = 0.0f;
	SphereComp = nullptr;
	MeshComp = nullptr;
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	BaseDamage = 0;
	CurrentDamage = 0;
}

void AUS_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
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
		ProjectileMovementComp->SetUpdatedComponent(RootComponent);
		ProjectileMovementComp->Velocity = GetActorForwardVector() * 7000.0f;
	}
}

void AUS_ProjectileBase::PushPoolSelf()
{
	if (ProjectilePool == nullptr)
	{
		return;
	}
	SetProjectileActive(false);
	ProjectilePool->PushProjectileInPool(this);
	UE_LOG(LogTemp, Warning, TEXT("Pool Self : %d"), ProjectilePool->Pool.Num());
}

void AUS_ProjectileBase::SetLifeSpanToPool()
{
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(TimerHandle_LifeSpanToPoolExpired, this, &AUS_ProjectileBase::PushPoolSelf, LifeSpanTime);
	}
}

