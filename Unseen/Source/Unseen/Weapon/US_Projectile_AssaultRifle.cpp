// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/US_Projectile_AssaultRifle.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/USBossBase.h"
#include "Weapon/BulletDamageCauser.h"

AUS_Projectile_AssaultRifle::AUS_Projectile_AssaultRifle()
{
	LifeSpanTime = 3.5f;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Bullet");
	SphereComp->SetWorldScale3D(FVector(0.1625f, 0.1625f, 0.1625f));
	SphereComp->SetSphereRadius(10.0f);
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionProfileName("NoCollision");
	MeshComp->SetWorldScale3D(FVector(0.2f, 0.5f, 0.2f));
	MeshComp->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	MeshComp->SetupAttachment(RootComponent);

	ProjectileMovementComp->InitialSpeed = 7000.0f;
	ProjectileMovementComp->MaxSpeed = 7000.0f;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;

	SphereComp->OnComponentHit.AddDynamic(this, &AUS_Projectile_AssaultRifle::OnHit);


	BaseDamage = 100;
	CurrentDamage = BaseDamage;
}

void AUS_Projectile_AssaultRifle::BeginPlay()
{
	Super::BeginPlay();

}

void AUS_Projectile_AssaultRifle::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Todo 面倒 单固瘤 贸府
	UE_LOG(LogTemp, Warning, TEXT("Bullet Hit %s"), *OtherComp->GetName());
	ABulletDamageCauser* DamageCauser = NewObject<ABulletDamageCauser>();
	DamageCauser->ComponentName = OtherComp->GetName();
	int Damage = FMath::RandRange(-10, 10) + CurrentDamage;
	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, OtherActor->GetActorLocation(), Hit, nullptr, DamageCauser, DamageType);
	//UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, DamageCauser, DamageType);

	//UE_LOG(LogTemp, Warning, TEXT("Bullet Hit %s"), *OtherActor->GetName());

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_LifeSpanToPoolExpired);
	PushPoolSelf();
}