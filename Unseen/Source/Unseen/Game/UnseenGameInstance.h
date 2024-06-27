// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UnseenGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API UUnseenGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	//Respawn
public:

	UUnseenGameInstance();

	UPROPERTY()
	int CurAmmo;
	UPROPERTY()
	int WeaponCurAmmo;
	UPROPERTY()
	int SRCnt;
	UPROPERTY()
	int VRCnt;
	UPROPERTY()
	int HRCnt;
	UPROPERTY()
	float CurMoney;
	UPROPERTY()
	float SRCost;
	UPROPERTY()
	float VRCost;
	UPROPERTY()
	float HRCost;
	UPROPERTY()
	float BulletPrice;
	
	UPROPERTY()
	float ShootRate;
	
	UPROPERTY()
	float CVR;
	UPROPERTY()
	float CHR;
	UPROPERTY()
	float HRA;

	UFUNCTION()
	void SetRespawnSet(int _CurAmmo, float _CurMoney, float _SRCost, float _VRCost, float _HRCost, float _BulletPrice, int _WeaponCurAmmo, float _ShootRate, float _CVR, float _CHR, float _HRA, int _SRCnt, int _VRCnt, int _HRCnt);
};
