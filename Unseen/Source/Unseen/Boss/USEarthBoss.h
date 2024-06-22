// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/USBossBase.h"
#include "AbilitySystemInterface.h"
#include "USEarthBoss.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API AUSEarthBoss : public AUSBossBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AUSEarthBoss();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void LimitBattleZone() override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

};
