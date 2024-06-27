// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/USBossBase.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Attribute/BossAttributeSet.h"
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

	const UBossAttributeSet* AttributeSet;

	UPROPERTY()
	TObjectPtr<class UBossAttributeSet> AttributeSetObject;

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BossBehaviorTree;
	
	/*UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BossBehaviorTree;*/


	UFUNCTION(BlueprintCallable)
	void SetSkillPos();

public:
	UPROPERTY(BlueprintReadWrite)
	FVector SkillPos;

	UPROPERTY(BlueprintReadWrite)
	FVector SpawnedPos;

	UFUNCTION()
	void OnRespawnCallback();
};
