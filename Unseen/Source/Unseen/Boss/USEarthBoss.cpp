// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/USEarthBoss.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

AUSEarthBoss::AUSEarthBoss()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetObject = CreateDefaultSubobject<UBossAttributeSet>(TEXT("AttributeSet"));

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

	MaxHp = 20000;
	CurrentHp = MaxHp;
}

UAbilitySystemComponent* AUSEarthBoss::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUSEarthBoss::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	for (const auto& StartInputAbility : StartInputAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		StartSpec.InputID = StartInputAbility.Key;
		ASC->GiveAbility(StartSpec);
	}
	AttributeSet = ASC->GetSet<UBossAttributeSet>();
}

void AUSEarthBoss::BeginPlay()
{
	Super::BeginPlay();

	SpawnedPos = GetActorLocation();
}

void AUSEarthBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AUSEarthBoss::LimitBattleZone()
{
	Super::LimitBattleZone();

	BattleZone = GetWorld()->SpawnActor<AActor>(BattleZoneBPClass, GetActorLocation(), FRotator::ZeroRotator);

	Cast<AAIController>(GetController())->RunBehaviorTree(BossBehaviorTree);
}

void AUSEarthBoss::SetSkillPos()
{
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector PlayerLocation = PlayerActor->GetActorLocation();

	int32 RandomValue1 = FMath::RandRange(0, 400);
	int32 RandomValue2 = FMath::RandRange(0, 400);

	SkillPos = FVector(PlayerLocation.X + RandomValue1, PlayerLocation.Y + RandomValue2, PlayerLocation.Z);
}

