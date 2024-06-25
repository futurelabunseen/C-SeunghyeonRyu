// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageWidgetActor.generated.h"

UCLASS()
class UNSEEN_API ADamageWidgetActor : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ADamageWidgetActor();

	void SetDamage(const float& InDamage, FLinearColor Color);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> DamageComponent;

	UFUNCTION()
	void ShowDamage(float InDamage, FLinearColor Color);
};
