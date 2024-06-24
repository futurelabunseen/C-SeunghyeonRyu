// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletDamageCauser.generated.h"

UCLASS()
class UNSEEN_API ABulletDamageCauser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletDamageCauser();
	
	UPROPERTY()
	FString ComponentName;

};
