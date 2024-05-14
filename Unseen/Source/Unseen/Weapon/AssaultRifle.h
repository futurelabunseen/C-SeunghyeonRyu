// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/UnseenWeaponBase.h"
#include "Animation/WeaponAnimInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AssaultRifle.generated.h"

UCLASS()
class UNSEEN_API AAssaultRifle : public AUnseenWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAssaultRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void ShootingMontageStart() override;
	void ShootingStop() override;
	void ShootWeapon() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Parts", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Aim;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Parts", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MainBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Parts", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Butt;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Parts", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RightKit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Parts", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Silencer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Parts", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Magazine;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Parts", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BulletSleeve;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMesh> AimSKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMesh> MainBodySKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMesh> ButtSKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMesh> RightKitSKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMesh> SilencerSKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMesh> MagazineSKMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkeletalMesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMesh> BulletSleeveSKMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim Instance", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWeaponAnimInstance> MainBodyAnimInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim Instance", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWeaponAnimInstance> MagazineAnimInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim Instance", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWeaponAnimInstance> BulletSleeveAnimInstance;




};
