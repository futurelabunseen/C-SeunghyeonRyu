// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectilePoolComponent_Base.h"

// Sets default values for this component's properties
UProjectilePoolComponent_Base::UProjectilePoolComponent_Base()
{
	InitialPoolSize = 0;
	ExpandSize = 0;
}

void UProjectilePoolComponent_Base::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	for (AUS_ProjectileBase* Projectile : Pool)
	{
		if (Projectile)
		{
			Projectile->Destroy();
		}
	}
}


// Called when the game starts
void UProjectilePoolComponent_Base::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectClass)
	{
		for (int i = 0; i < InitialPoolSize; i++)
		{
			AUS_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AUS_ProjectileBase>(PooledObjectClass, FVector().ZeroVector, FRotator().ZeroRotator);
			SpawnedProjectile->SetProjectileActive(false);
			SpawnedProjectile->ProjectilePool = this;
			Pool.Push(SpawnedProjectile);
		}
	}
}

void UProjectilePoolComponent_Base::PushProjectileInPool(AUS_ProjectileBase* Projectile)
{
	Pool.Push(Projectile);
}

AUS_ProjectileBase* UProjectilePoolComponent_Base::PullProjectile()
{
	if (Pool.Num() == 0)
	{
		Expand();
	}

	return Pool.Pop();
}

void UProjectilePoolComponent_Base::Expand()
{
	if (PooledObjectClass)
	{
		for (int i = 0; i < ExpandSize; i++)
		{
			AUS_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AUS_ProjectileBase>(PooledObjectClass, FVector().ZeroVector, FRotator().ZeroRotator);
			SpawnedProjectile->SetProjectileActive(false);
			Pool.Push(SpawnedProjectile);
		}
	}
}

