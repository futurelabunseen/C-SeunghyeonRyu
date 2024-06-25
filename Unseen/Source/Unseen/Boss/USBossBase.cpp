// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/USBossBase.h"
#include "UI/BossFightHUD.h"
#include "Weapon/BulletDamageCauser.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UI/DamageWidgetActor.h"
#include "Character/UnseenCharacterPlayer.h"
#include "Engine/DamageEvents.h"

// Sets default values
AUSBossBase::AUSBossBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsBattleStart = false;
	MaxHp = 0;
	CurrentHp = MaxHp;
	BossFightHUDClass = nullptr;
	BossFightHUD = nullptr;
	BattleZoneBPClass = nullptr;
	BattleZone = nullptr;
	PlayerCharacterPtr = nullptr;

	static ConstructorHelpers::FObjectFinder<USoundCue> HitAudioObject(TEXT("/Script/Engine.SoundCue'/Game/Vefects/Shots_VFX/Audio/SFX_Vefects_Shots_Squib_Dirt_Cue.SFX_Vefects_Shots_Squib_Dirt_Cue'"));
	if (nullptr != HitAudioObject.Object)
	{
		HitSound = HitAudioObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> CriticalAudioObject(TEXT("/Script/Engine.SoundCue'/Game/Vefects/Shots_VFX/Audio/SFX_Vefects_Shots_Squib_Metal_Cue.SFX_Vefects_Shots_Squib_Metal_Cue'"));
	if (nullptr != CriticalAudioObject.Object)
	{
		CriticalHitSound = CriticalAudioObject.Object;
	}
	
}

// Called when the game starts or when spawned
void AUSBossBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUSBossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsBattleStart)
	{
		FVector CharacterLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		float Distance = FVector::Dist(GetActorLocation(), CharacterLocation);
		if (Distance < 3000.0f)
		{
			bIsBattleStart = true;
			LimitBattleZone();
		}
		// 나중에 투명벽 하나 만들어놓고 델리게이트로 구현
	}

}

// Called to bind functionality to input
void AUSBossBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUSBossBase::LimitBattleZone()
{
	PlayerCharacterPtr = CastChecked<AUnseenCharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UE_LOG(LogTemp, Warning, TEXT("Create Battle Zone"));
	// 벽 동그랗게 만들어둔 액터 스폰
	if (BossFightHUDClass)
	{
		BossFightHUD = CreateWidget<UBossFightHUD>(GetWorld()->GetFirstPlayerController(), BossFightHUDClass);
		if (nullptr != BossFightHUD)
		{
			BossFightHUD->SetHealthBar(CurrentHp, MaxHp);
			BossFightHUD->AddToPlayerScreen();
		}
	}
}

float AUSBossBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsBattleStart)
	{
		FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		float Damage;
		ABulletDamageCauser* CustomCauser = Cast<ABulletDamageCauser>(DamageCauser);
		
		if (CustomCauser->ComponentName == TEXT("WeakPoint"))
		{
			Damage = DamageAmount * 1.5f;
			UGameplayStatics::PlaySound2D(this, CriticalHitSound);
			GetWorld()->SpawnActor<ADamageWidgetActor>(ADamageWidgetActor::StaticClass(), PointDamageEvent->HitInfo.ImpactPoint, FRotator::ZeroRotator)->SetDamage(Damage, FLinearColor::Red);
		}
		else
		{
			Damage = DamageAmount;
			UGameplayStatics::PlaySound2D(this, HitSound);
			GetWorld()->SpawnActor<ADamageWidgetActor>(ADamageWidgetActor::StaticClass(), PointDamageEvent->HitInfo.ImpactPoint, FRotator::ZeroRotator)->SetDamage(Damage, FLinearColor::Blue);
		}
		
		if (CurrentHp <= Damage)
		{
			CurrentHp = 0;
			UE_LOG(LogTemp, Warning, TEXT("Boss Die"));
			
			if (BossFightHUD)
			{
				BossFightHUD->RemoveFromParent();
				BossFightHUD = nullptr;
			}
			if (BattleZone)
			{
				BattleZone->Destroy();
			}
			Destroy();
		}
		else
		{
			CurrentHp -= Damage;
			UE_LOG(LogTemp, Warning, TEXT("Boss Hp : %d"), CurrentHp);
		}

		if (BossFightHUD)
		{
			BossFightHUD->SetHealthBar(CurrentHp, MaxHp);
		}

		float ResultMoney = PlayerCharacterPtr->Money + Damage;
		ResultMoney = ResultMoney > PlayerCharacterPtr->MaxMoney ? PlayerCharacterPtr->MaxMoney : ResultMoney;
		PlayerCharacterPtr->Money = ResultMoney;

		return Damage;
	}
	return 0.0f;
}

