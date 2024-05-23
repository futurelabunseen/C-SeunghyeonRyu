// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/USEarthBoss.h"
#include "Blueprint/UserWidget.h"

AUSEarthBoss::AUSEarthBoss()
{
	static ConstructorHelpers::FClassFinder<AActor> BattleZoneBPClassRef(TEXT("/Script/Engine.Blueprint'/Game/Boss/BattleZoneWall.BattleZoneWall_C'"));
	if (BattleZoneBPClassRef.Class)
	{
		BattleZoneBPClass = BattleZoneBPClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> BossFightHUDClassRef(TEXT("/Game/UI/WBP_BossFight.WBP_BossFight_C"));
	if (BossFightHUDClassRef.Class)
	{
		BossFightHUDClass = BossFightHUDClassRef.Class;
	}

	MaxHp = 10000;
	CurrentHp = MaxHp;
}

void AUSEarthBoss::BeginPlay()
{
	Super::BeginPlay();
}

void AUSEarthBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AUSEarthBoss::LimitBattleZone()
{
	Super::LimitBattleZone();

	BattleZone = GetWorld()->SpawnActor<AActor>(BattleZoneBPClass, GetActorLocation(), FRotator::ZeroRotator);

	// ui ¶ç¿ì±â
}

