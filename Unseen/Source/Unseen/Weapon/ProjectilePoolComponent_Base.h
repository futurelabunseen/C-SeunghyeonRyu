// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/US_ProjectileBase.h"
#include "ProjectilePoolComponent_Base.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNSEEN_API UProjectilePoolComponent_Base : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectilePoolComponent_Base();

	void OnComponentDestroyed(bool bDestroyingHierarchy) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UFUNCTION()
	void PushProjectileInPool(AUS_ProjectileBase* Projectile);

	UFUNCTION()
	AUS_ProjectileBase* PullProjectile();

	UPROPERTY(VisibleAnywhere)
	TArray<AUS_ProjectileBase*> Pool;
protected:

	// ó�� ��������� ������
	UPROPERTY()
	int InitialPoolSize;
	// 0���϶� pull�ϸ� ��������� ����
	UPROPERTY()
	int ExpandSize;

	UPROPERTY()
	TSubclassOf<AUS_ProjectileBase> PooledObjectClass;

	UFUNCTION()
	void Expand();
};
