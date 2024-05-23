// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectilePool_AssaultRifle.h"


AProjectilePool_AssaultRifle::AProjectilePool_AssaultRifle()
{
	InitialPoolSize = 30;
	ExpandSize = 1;

	static ConstructorHelpers::FClassFinder<AActor> PooledObjectClassRef(TEXT("/Script/Engine.Blueprint'/Game/Weapon/USBP_Projectile_AssaultRifle.USBP_Projectile_AssaultRifle_C'"));
	if (PooledObjectClassRef.Class)
	{
		PooledObjectClass = PooledObjectClassRef.Class;
	}
}