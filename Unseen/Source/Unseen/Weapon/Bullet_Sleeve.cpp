// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet_Sleeve.h"

// Sets default values
ABullet_Sleeve::ABullet_Sleeve()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletSleeveStaticMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Weapon/SM_Sleeve.SM_Sleeve'"));
	if (nullptr != BulletSleeveStaticMeshRef.Object)
	{
		BulletSleeveStaticMesh = BulletSleeveStaticMeshRef.Object;
	}

	BulletSleeveMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("S_Sleeve"));
	RootComponent = BulletSleeveMeshComponent;

	BulletSleeveMeshComponent->SetStaticMesh(BulletSleeveStaticMesh);
	BulletSleeveMeshComponent->SetSimulatePhysics(true);
	BulletSleeveMeshComponent->SetCollisionProfileName(TEXT("Sleeve"));
}

// Called when the game starts or when spawned
void ABullet_Sleeve::BeginPlay()
{
	Super::BeginPlay();

	FVector temp = GetActorTransform().TransformVector(FVector(0.0f, -80.0f, 0.0f));
	BulletSleeveMeshComponent->AddImpulse(temp, NAME_None, true);
	//BulletSleeveMeshComponent->AddImpulse(FVector(0.0f, 80.0f, 0.0f), NAME_None, true);
	SetLifeSpan(5.0f);
}


