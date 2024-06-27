// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AssaultRifle.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
AAssaultRifle::AAssaultRifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AimSKMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Sci-Fi_Assault_Rifle/Assault_Rifle/SK_Rifle_Mesh/SK_Aim.SK_Aim'"));
	if (nullptr != AimSKMeshRef.Object)
	{
		AimSKMesh = AimSKMeshRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AssaultRifleSKMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Sci-Fi_Assault_Rifle/Assault_Rifle/SK_Rifle_Mesh/SK_Rifle_Assault_Rifle.SK_Rifle_Assault_Rifle'"));
	if (nullptr != AssaultRifleSKMeshRef.Object)
	{
		MainBodySKMesh = AssaultRifleSKMeshRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ButtSKMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Sci-Fi_Assault_Rifle/Assault_Rifle/SK_Rifle_Mesh/SK_Butt.SK_Butt'"));
	if (nullptr != ButtSKMeshRef.Object)
	{
		ButtSKMesh = ButtSKMeshRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RightKitSKMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Sci-Fi_Assault_Rifle/Assault_Rifle/SK_Rifle_Mesh/SK_Right_Kit.SK_Right_Kit'"));
	if (nullptr != RightKitSKMeshRef.Object)
	{
		RightKitSKMesh = RightKitSKMeshRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SilencerSKMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Sci-Fi_Assault_Rifle/Assault_Rifle/SK_Rifle_Mesh/SK_Silencer.SK_Silencer'"));
	if (nullptr != SilencerSKMeshRef.Object)
	{
		SilencerSKMesh = SilencerSKMeshRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MagazineSKMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Sci-Fi_Assault_Rifle/Assault_Rifle/SK_Rifle_Mesh/SK_Magazine.SK_Magazine'"));
	if (nullptr != MagazineSKMeshRef.Object)
	{
		MagazineSKMesh = MagazineSKMeshRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BulletSleeveSKMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Sci-Fi_Assault_Rifle/Assault_Rifle/SK_Rifle_Mesh/SK_Sleeve_AK47.SK_Sleeve_AK47'"));
	if (nullptr != BulletSleeveSKMeshRef.Object)
	{
		BulletSleeveSKMesh = BulletSleeveSKMeshRef.Object;
	}
	//Sound
	static ConstructorHelpers::FObjectFinder<USoundCue> ShootSoundObject(TEXT("/Script/Engine.SoundCue'/Game/Vefects/Shots_VFX/Audio/SFX_Vefects_Shots_Weapon_Rifle_Cue.SFX_Vefects_Shots_Weapon_Rifle_Cue'"));
	if (nullptr != ShootSoundObject.Object)
	{
		ShootSound = ShootSoundObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MuzzleFlashObject(TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Shots_VFX/VFX/MuzzleFlash/FX_MuzzleFlash_Rifle.FX_MuzzleFlash_Rifle'"));
	if (nullptr != MuzzleFlashObject.Object)
	{
		MuzzleFlashEffect = MuzzleFlashObject.Object;
	}

	//SkeletalMeshComponent
	Aim = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Aim"));
	MainBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Assault_Rifle"));
	Butt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Butt"));
	RightKit = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Right_Kit"));
	Silencer = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Silencer"));
	Magazine = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Magazine_Full"));
	BulletSleeve = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_BulletSleeve"));

	// 나중에 파츠 구매로 바꿔도 될 듯
	RootComponent = MainBody;
	Aim->SetupAttachment(RootComponent);
	Butt->SetupAttachment(RootComponent);
	RightKit->SetupAttachment(RootComponent);
	Silencer->SetupAttachment(RootComponent);
	Magazine->SetupAttachment(RootComponent);
	BulletSleeve->SetupAttachment(RootComponent);

	MainBody->SetSkeletalMeshAsset(MainBodySKMesh);
	Aim->SetSkeletalMeshAsset(AimSKMesh);
	Butt->SetSkeletalMeshAsset(ButtSKMesh);
	RightKit->SetSkeletalMeshAsset(RightKitSKMesh);
	Silencer->SetSkeletalMeshAsset(SilencerSKMesh);
	Magazine->SetSkeletalMeshAsset(MagazineSKMesh);
	BulletSleeve->SetSkeletalMeshAsset(BulletSleeveSKMesh);

	MainBody->SetCollisionProfileName(TEXT("NoCollision"));
	Aim->SetCollisionProfileName(TEXT("NoCollision"));
	Butt->SetCollisionProfileName(TEXT("NoCollision"));
	RightKit->SetCollisionProfileName(TEXT("NoCollision"));
	Silencer->SetCollisionProfileName(TEXT("NoCollision"));
	Magazine->SetCollisionProfileName(TEXT("NoCollision"));
	BulletSleeve->SetCollisionProfileName(TEXT("NoCollision"));

	Aim->SetRelativeLocation(FVector(-6.31f, 0, 12.67f));
	Butt->SetRelativeLocation(FVector(1.51f, 0, 7.15f));
	RightKit->SetRelativeLocation(FVector(-36.47f, -2.3f, 7.69f));
	Silencer->SetRelativeLocation(FVector(-62.28f, 0, 7.21f));
	Magazine->SetRelativeLocation(FVector(-16.63f, 0, 4.99f));
	BulletSleeve->SetRelativeLocation(FVector(-25.77f, 0, 9.59f));

	MaxAmmo = 30;

	MaxVerticalRecoil = 2.0f;
	MaxHorizontalRecoil = 40.0f; // 10
	CurrentVerticalRecoil = MaxVerticalRecoil;
	CurrentHorizontalRecoil = 0.0f;
	HorizontalRecoilAmount = 4.0f; // 1
	CurrentAmmo = MaxAmmo;
	ShootRate = 1.0f;

	ProjectilePool = CreateDefaultSubobject<UPoolComponent_AssaultRifle>(TEXT("ProjectilePool"));

}

// Called when the game starts or when spawned
void AAssaultRifle::BeginPlay()
{
	Super::BeginPlay();

	MainBodyAnimInstance = CastChecked<UWeaponAnimInstance>(MainBody->GetAnimInstance());
	MagazineAnimInstance = CastChecked<UWeaponAnimInstance>(Magazine->GetAnimInstance());
	BulletSleeveAnimInstance = CastChecked<UWeaponAnimInstance>(BulletSleeve->GetAnimInstance());


}

// Called every frame
void AAssaultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsOnHand)
	{
		if (CurrentHorizontalRecoil > 0.0f)
		{
			CurrentHorizontalRecoil = FMath::FInterpTo(CurrentHorizontalRecoil, 0.0f, DeltaTime, 0.5f);
			OwnerPlayerHUD->SetHorizontalCrossHairPos(CurrentHorizontalRecoil);
		}
	}
	

}

void AAssaultRifle::ShootingMontageStart()
{
	Super::ShootingMontageStart();

	MainBodyAnimInstance->PlayShootMontage(EWeaponPart::MainBody);
	//BulletSleeveAnimInstance->PlayShootMontage(EWeaponPart::BulletSleeve);
}

void AAssaultRifle::ShootingStop()
{
	Super::ShootingStop();

	MainBodyAnimInstance->StopShootMontage(EWeaponPart::MainBody);
	//BulletSleeveAnimInstance->StopShootMontage(EWeaponPart::BulletSleeve);
}

void AAssaultRifle::ShootWeapon()
{
	Super::ShootWeapon();
	
	UGameplayStatics::PlaySound2D(this, ShootSound);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		MuzzleFlashEffect,
		Silencer->GetSocketLocation(FName("Muzzle")),
		Silencer->GetSocketRotation(FName("Muzzle"))
	);

	CurrentAmmo -= 1;
	GetWorld()->GetFirstPlayerController()->GetPawn()->AddControllerPitchInput(-CurrentVerticalRecoil);
	GetWorld()->SpawnActor<ABullet_Sleeve>(ABullet_Sleeve::StaticClass(), MainBody->GetSocketLocation(FName("Sleeve")), MainBody->GetSocketRotation(FName("Sleeve")));
	
	float AmountHorizontalRecoil = MaxHorizontalRecoil - CurrentHorizontalRecoil < HorizontalRecoilAmount ? MaxHorizontalRecoil - CurrentHorizontalRecoil : HorizontalRecoilAmount;
	CurrentHorizontalRecoil += AmountHorizontalRecoil;
	ChangeMaterialBulletVariable();
	
	//총 발사 로직

	
}

void AAssaultRifle::ShootWeaponNotHorizontalRecoil()
{
	Super::ShootWeaponNotHorizontalRecoil();

	CurrentAmmo -= 1;
	GetWorld()->SpawnActor<ABullet_Sleeve>(ABullet_Sleeve::StaticClass(), MainBody->GetSocketLocation(FName("Sleeve")), MainBody->GetSocketRotation(FName("Sleeve")));

	float AmountHorizontalRecoil = MaxHorizontalRecoil - CurrentHorizontalRecoil < HorizontalRecoilAmount ? MaxHorizontalRecoil - CurrentHorizontalRecoil : HorizontalRecoilAmount;
	CurrentHorizontalRecoil += AmountHorizontalRecoil;
	ChangeMaterialBulletVariable();
	UE_LOG(LogTemp, Warning, TEXT("Current Ammo : %d"), CurrentAmmo);
}

FVector AAssaultRifle::GetMuzzlePos()
{
	return Silencer->GetSocketLocation(FName("Muzzle"));
}

//void AAssaultRifle::RespawnCharacterSet(int WeaponCurAmmo, float _ShootRate, float CVR, float CHR, float HRA)
//{
//	Super::RespawnCharacterSet(WeaponCurAmmo, _ShootRate, CVR, CHR, HRA);
//}


