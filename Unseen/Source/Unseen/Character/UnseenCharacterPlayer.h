// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UnseenCharacterBase.h"
#include "InputActionValue.h"
#include "UnseenCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API AUnseenCharacterPlayer : public AUnseenCharacterBase
{
	GENERATED_BODY()
	
public:
	AUnseenCharacterPlayer();

protected:
	virtual void BeginPlay();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	void Move(const FInputActionValue& Value);
	void StopMoving();

	void Look(const FInputActionValue& Value);

};
