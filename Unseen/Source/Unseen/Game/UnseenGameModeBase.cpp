// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnseenGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Character/UnseenCharacterPlayer.h"
#include "UI/PauseMenu.h"

AUnseenGameModeBase::AUnseenGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/Characters/GASUnseen/BP_UnseenCharacterPlayer"));
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Script/Unseen.UnseenCharacterPlayer"));
	if (ThirdPersonClassRef.Class != nullptr)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Unseen.UnseenPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateClassRef(TEXT("/Script/Unseen.UnseenPlayerState"));
	if (PlayerStateClassRef.Class)
	{
		PlayerStateClass = PlayerStateClassRef.Class;
	}
	PlayerController = nullptr;

	static ConstructorHelpers::FClassFinder<AActor> BossClassRef(TEXT("/Game/Boss/BP_USEarthBoss.BP_USEarthBoss_C"));
	if (BossClassRef.Class)
	{
		BossMonsterClass = BossClassRef.Class;
	}
}



void AUnseenGameModeBase::RespawnPlayer()
{
	OnRespawn.Broadcast();
	SpawnBoss();
	PlayerController = GetWorld()->GetFirstPlayerController();
	AUnseenCharacterPlayer* PlayerCharacter = CastChecked<AUnseenCharacterPlayer>(PlayerController->GetCharacter());
	AActor* PlayerStart = FindPlayerStart(PlayerController);
	FVector PlayerStartPos = PlayerStart->GetActorLocation();
	FRotator PlayerStartRot = PlayerStart->GetActorRotation();

	if (PlayerCharacter)
	{
		// Legend Code...
		int a = PlayerCharacter->CharacterCurrentAmmo;
		float b = PlayerCharacter->Money;
		float c = PlayerCharacter->ShootRateMoney;
		float d = PlayerCharacter->VerticalRecoilMoney;
		float e = PlayerCharacter->HorizontalRecoilMoney;
		float f = PlayerCharacter->BulletMoney;
		int g = PlayerCharacter->WeaponOnHand->CurrentAmmo;
		float h = PlayerCharacter->WeaponOnHand->ShootRate;
		float i = PlayerCharacter->WeaponOnHand->CurrentVerticalRecoil;
		float j = PlayerCharacter->WeaponOnHand->CurrentHorizontalRecoil;
		float k = PlayerCharacter->WeaponOnHand->HorizontalRecoilAmount;
		int l = PlayerCharacter->PauseMenuWidget->ShootRateCnt;
		int m = PlayerCharacter->PauseMenuWidget->VerticalCnt;
		int n = PlayerCharacter->PauseMenuWidget->HorizontalCnt;

		AUnseenCharacterPlayer* NewCharacter = GetWorld()->SpawnActor<AUnseenCharacterPlayer>(DefaultPawnClass, PlayerStartPos, PlayerStartRot);
		PlayerCharacter->UnPossessed();
		PlayerCharacter->RestartUI->RemoveFromParent();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		PlayerCharacter->GetWeaponOnHand()->ProjectilePool->DestroyComponent();
		PlayerCharacter->GetWeaponOnHand()->Destroy();
		PlayerCharacter->Destroy();
		//PlayerController->Possess(NewCharacter);
		
		NewCharacter->RespawnCharacterSet(a, b, c, d, e, f, g, h, i, j, k, l, m, n);
		
	}
}

void AUnseenGameModeBase::RestartGame()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FName CurrentLevelName = FName(World->GetMapName());
		UGameplayStatics::OpenLevel(World, CurrentLevelName);
	}
}

void AUnseenGameModeBase::SpawnBoss()
{
	FVector BossPos = FVector(3200.f, 0.f, 88.f);
	FRotator BossRot = FRotator(0.f, 180.f, 0.f);
	GetWorld()->SpawnActor<AActor>(BossMonsterClass, BossPos, BossRot);
}
