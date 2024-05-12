// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/WeaponAnimInstance.h"


UWeaponAnimInstance::UWeaponAnimInstance()
{
	bIsWeapon = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MainBodyShootMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Weapon/AnimMontage/AM_Shooting_Automatic_Assault_Rifle.AM_Shooting_Automatic_Assault_Rifle'"));
	if (MainBodyShootMontageRef.Succeeded())
	{
		MainBodyShootMontage = MainBodyShootMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SleeveShootMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Weapon/AnimMontage/AM_Sleeve_Shooting_Automatic.AM_Sleeve_Shooting_Automatic'"));
	if (SleeveShootMontageRef.Succeeded())
	{
		SleeveShootMontage = SleeveShootMontageRef.Object;
	}

	ShootRate = 1.0f;
}

void UWeaponAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Weapon = Cast<AUnseenWeaponBase>(GetOwningActor());
	if (Weapon)
	{
		bIsWeapon = true;
		UpdateShootRate();
	}
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (bIsWeapon)
	{
		bIsShooting = Weapon->bIsShooting;
	}
}

void UWeaponAnimInstance::PlayShootMontage(EWeaponPart part)
{
	switch (part)
	{
	case EWeaponPart::MainBody:
		Montage_Play(MainBodyShootMontage, ShootRate);
		break;
	case EWeaponPart::BulletSleeve:
		Montage_Play(SleeveShootMontage, ShootRate);
		break;

	}
}

void UWeaponAnimInstance::StopShootMontage()
{
	Montage_Stop(0.0f);
}

void UWeaponAnimInstance::UpdateShootRate()
{
	ShootRate = Weapon->ShootRate;
}
