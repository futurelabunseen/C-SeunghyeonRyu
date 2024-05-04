// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UnseenCharacterBase.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "Components/TimelineComponent.h"
#include "UnseenCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNSEEN_API AUnseenCharacterPlayer : public AUnseenCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AUnseenCharacterPlayer();
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

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
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> StepBackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimAction;

	void Move(const FInputActionValue& Value);
	//void StopMoving();

	void Look(const FInputActionValue& Value);

	//void Sprint();
	//void StopSprinting();

// Ability System
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	TObjectPtr<class USpringArmComponent> GetSpringArmComponent();

public:
	FORCEINLINE class UAnimMontage* GetRollMontage() const { return RollMontage; }
	FORCEINLINE class UAnimMontage* GetStepBackMontage() const { return StepBackMontage; }

protected:
	void SetupGASInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> RegenStaminaEffect;
	
// Montage
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> StepBackMontage;

// Montage Timeline
	
// Roll
	UPROPERTY()
	TObjectPtr<class UTimelineComponent> RollMovementTimeline;

	UPROPERTY()
	TObjectPtr<class UCurveFloat> RollMovementCurve;

	UFUNCTION()
	void OnRollMovementTimelineUpdated(float Value);

	FOnTimelineFloat RollTimeLineInterpFunction{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float RollDistance;

// StepBack
	UPROPERTY()
	TObjectPtr<class UTimelineComponent> StepBackMovementTimeline;

	UPROPERTY()
	TObjectPtr<class UCurveFloat> StepBackMovementCurve;

	UFUNCTION()
	void OnStepBackMovementTimelineUpdated(float Value);

	FOnTimelineFloat StepBackTimeLineInterpFunction{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float StepBackDistance;


public:
	UFUNCTION()
	UTimelineComponent* GetRollMovementTimeline();

	UFUNCTION()
	UTimelineComponent* GetStepBackMovementTimeline();

protected:
	UFUNCTION()
	void OnRollStepBackCameraTimelineUpdated(float Value);

	UFUNCTION()
	void OnAimCameraTimelineUpdated(float Value);

	UPROPERTY()
	TObjectPtr<class UCurveFloat> RollStepBackCameraCurve;

	UPROPERTY()
	TObjectPtr<class UCurveFloat> AimCameraCurve;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> RollStepBackCameraTimeline;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> AimCameraTimeline;

	FOnTimelineFloat RollStepBackCameraTimeLineInterpFunction{};
	FOnTimelineFloat AimCameraTimeLineInterpFunction{};

	UPROPERTY()
	FVector2D LastInputMoveValue;

	UPROPERTY()
	float CurrentTargetArmLength;
	UPROPERTY()
	FVector CurrentSpringArmSocketOffset;

public:
	UFUNCTION()
	void RollStepBackCameraLerp();

	UFUNCTION()
	void AimCameraLerp();

	UFUNCTION()
	FVector2D GetLastInputMoveValue();

	UFUNCTION()
	void UpdateCurrentTargetArmLength();
	UFUNCTION()
	void UpdateCurrentSpringArmSocketOffset();

	UPROPERTY()
	uint8 bIsRollStepBackActive : 1;

	UPROPERTY()
	uint8 bIsAiming : 1;

	UPROPERTY()
	uint8 bIsSprinting : 1;

	UFUNCTION()
	void StopRegenStamina();

	UFUNCTION()
	void StartRegenStaminaWithDelay(float DelayTime);

	UFUNCTION()
	void StartRegenStamina();
	UPROPERTY()
	uint8 bIsBlockedRegenStamina : 1;

	UFUNCTION()
	FVector CalculateRollDirection();

	// HUD
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUnseenCharacterHUD> PlayerHUDClass;

	UPROPERTY()
	TObjectPtr<class UUnseenCharacterHUD> PlayerHUD;

	// Weapon
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUnseenCharacterHUD> AssultRifleClass;

	UPROPERTY()
	TObjectPtr<class UUnseenCharacterHUD> AssultRifle;
};
