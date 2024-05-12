// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapon/UnseenWeaponBase.h"
#include "WeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UWeaponAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class AUnseenWeaponBase> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float ShootRate;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	uint8 bIsWeapon : 1;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	uint8 bIsShooting : 1;*/

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> MainBodyShootMontage;
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> SleeveShootMontage;

public:
	void PlayShootMontage(EWeaponPart part);
	void StopShootMontage(EWeaponPart part);
	void UpdateShootRate();
};
