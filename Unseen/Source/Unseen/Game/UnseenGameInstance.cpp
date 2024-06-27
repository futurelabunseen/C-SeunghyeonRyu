// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UnseenGameInstance.h"

UUnseenGameInstance::UUnseenGameInstance()
{
	CurAmmo = 150;
	CurMoney = 1000.0f;
	SRCost = 500.0f;
	VRCost = 500.0f;
	HRCost = 500.0f;
	BulletPrice = 500.0f;
	WeaponCurAmmo = 30;
	ShootRate = 1.0f;
	CVR = 2.0f;
	CHR = 0.0f;
	HRA = 4.0f;
	SRCnt = 0;
	VRCnt = 0;
	HRCnt = 0;
}

void UUnseenGameInstance::SetRespawnSet(int _CurAmmo, float _CurMoney, float _SRCost, float _VRCost, float _HRCost, float _BulletPrice, int _WeaponCurAmmo, float _ShootRate, float _CVR, float _CHR, float _HRA, int _SRCnt, int _VRCnt, int _HRCnt)
{
	CurAmmo = _CurAmmo;
	CurMoney = _CurMoney;
	SRCost = _SRCost;
	VRCost = _VRCost;
	HRCost = _HRCost;
	BulletPrice = _BulletPrice;
	WeaponCurAmmo = _WeaponCurAmmo;
	ShootRate = _ShootRate;
	CVR = _CVR;
	CHR = _CHR;
	HRA = _HRA;
	SRCnt = _SRCnt;
	VRCnt = _VRCnt;
	HRCnt = _HRCnt;
}
