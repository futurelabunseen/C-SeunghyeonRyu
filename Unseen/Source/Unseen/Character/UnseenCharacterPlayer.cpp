// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UnseenCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "Player/UnseenPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/SpringArmComponent.h"

AUnseenCharacterPlayer::AUnseenCharacterPlayer()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 40.f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Get Ability System from PlayerState
	ASC = nullptr;

	bIsRollStepBackActive = true;
	bIsBlockedRegenStamina = false;
	bIsSprinting = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ThirdPerson/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Sprint.IA_Sprint'"));
	if (nullptr != InputActionSprintRef.Object)
	{
		SprintAction = InputActionSprintRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRollRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Roll.IA_Roll'"));
	if (nullptr != InputActionRollRef.Object)
	{
		RollAction = InputActionRollRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionStepBackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_StepBack.IA_StepBack'"));
	if (nullptr != InputActionStepBackRef.Object)
	{
		StepBackAction = InputActionStepBackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAimRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Aim.IA_Aim'"));
	if (nullptr != InputActionAimRef.Object)
	{
		AimAction = InputActionAimRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RollMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Roll_Rifle.AM_Roll_Rifle'"));
	if (RollMontageRef.Succeeded())
	{
		RollMontage = RollMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StepBackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Step_Back.AM_Step_Back'"));
	if (StepBackMontageRef.Succeeded())
	{
		StepBackMontage = StepBackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> RollMovementCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/Animation/FloatCurve_Roll.FloatCurve_Roll'"));
	if (RollMovementCurveRef.Object)
	{
		RollMovementCurve = RollMovementCurveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> StepBackMovementCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/Animation/FloatCurve_StepBack.FloatCurve_StepBack'"));
	if (StepBackMovementCurveRef.Object)
	{
		StepBackMovementCurve = StepBackMovementCurveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> RollStepBackCameraCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/Curve/FloatCurve_RollStepBackCamera.FloatCurve_RollStepBackCamera'"));
	if (RollStepBackCameraCurveRef.Object)
	{
		RollStepBackCameraCurve = RollStepBackCameraCurveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> AimCameraCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/Curve/FloatCurve_AimCamera.FloatCurve_AimCamera'"));
	if (AimCameraCurveRef.Object)
	{
		AimCameraCurve = AimCameraCurveRef.Object;
	}

	RollMovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RollMovementTimeline"));
	RollDistance = 400.f;

	StepBackMovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("StepBackMovementTimeline"));
	StepBackDistance = -650.f; // not real distance.

	RollStepBackCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RollStepBackCameraTimeline"));

	AimCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimCameraTimeline"));

}

void AUnseenCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AUnseenPlayerState* UnseenPlayerState = GetPlayerState<AUnseenPlayerState>();
	if (UnseenPlayerState)
	{
		ASC = UnseenPlayerState->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(UnseenPlayerState, this);

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		SetupGASInputComponent();
	}
	
	APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

	const UGameplayEffect* RegenStaminaEffectCDO = RegenStaminaEffect->GetDefaultObject<UGameplayEffect>();
	ASC->ApplyGameplayEffectToSelf(RegenStaminaEffectCDO, 0, ASC->MakeEffectContext());

}

void AUnseenCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Only for Player, So use CastChecked
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
	}

	// Roll Movement Timeline
	RollTimeLineInterpFunction.BindUFunction(this, FName{ TEXT("OnRollMovementTimelineUpdated") });
	RollMovementTimeline->AddInterpFloat(RollMovementCurve, RollTimeLineInterpFunction);
	RollMovementTimeline->SetTimelineLength(RollMontage->GetPlayLength());
	RollMovementTimeline->SetLooping(false);
	
	// StepBack Movement Timeline
	StepBackTimeLineInterpFunction.BindUFunction(this, FName{ TEXT("OnStepBackMovementTimelineUpdated") });
	StepBackMovementTimeline->AddInterpFloat(StepBackMovementCurve, StepBackTimeLineInterpFunction);
	StepBackMovementTimeline->SetTimelineLength(StepBackMontage->GetPlayLength());
	StepBackMovementTimeline->SetLooping(false);

	// Roll StepBack Camera Timeline
	RollStepBackCameraTimeLineInterpFunction.BindUFunction(this, FName{ TEXT("OnRollStepBackCameraTimelineUpdated") });
	RollStepBackCameraTimeline->AddInterpFloat(RollStepBackCameraCurve, RollStepBackCameraTimeLineInterpFunction);
	RollStepBackCameraTimeline->SetTimelineLength(0.4f);
	RollStepBackCameraTimeline->SetLooping(false);

	// Aim Camera Timeline
	AimCameraTimeLineInterpFunction.BindUFunction(this, FName{ TEXT("OnAimCameraTimelineUpdated") });
	AimCameraTimeline->AddInterpFloat(AimCameraCurve, AimCameraTimeLineInterpFunction);
	AimCameraTimeline->SetTimelineLength(0.3f);
	AimCameraTimeline->SetLooping(false);

}

void AUnseenCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::Move);
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::StopMoving);

		//Sprinting
		//EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::Sprint);
		//EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::StopSprinting);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::Look);
	}
}

void AUnseenCharacterPlayer::Move(const FInputActionValue& Value)
{
	LastInputMovementVector = Value.Get<FVector2D>();
	

	//FGameplayTagContainer CurrentOwnedGameplayTags;
	//ASC->GetOwnedGameplayTags(CurrentOwnedGameplayTags);
	//if (CurrentOwnedGameplayTags.HasTag(FGameplayTag::RequestGameplayTag("Character.State.IsMoving")))
	//{
	//	// input is a Vector2D
	//	FVector2D MovementVector = Value.Get<FVector2D>();

	//	if (Controller != nullptr)
	//	{
	//		// find out which way is forward
	//		const FRotator Rotation = Controller->GetControlRotation();
	//		const FRotator YawRotation(0, Rotation.Yaw, 0);

	//		// get forward vector
	//		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//		// get right vector 
	//		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//		// add movement 
	//		AddMovementInput(ForwardDirection, MovementVector.X);
	//		AddMovementInput(RightDirection, MovementVector.Y);
	//	}
	//}
}

//void AUnseenCharacterPlayer::StopMoving()
//{
//	///bUseControllerRotationYaw = false;
//}

void AUnseenCharacterPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//void AUnseenCharacterPlayer::Sprint()
//{
//	FVector CharacterForward = GetActorForwardVector();
//	FVector VelocityDirection = GetVelocity().GetSafeNormal();
//	float MovementDirection = FVector::DotProduct(CharacterForward, VelocityDirection);
//
//	if (!GetCharacterMovement()->Velocity.IsNearlyZero() && MovementDirection > 0.5)
//	{
//		GetCharacterMovement()->MaxWalkSpeed = 500.f;
//	}
//}
//
//void AUnseenCharacterPlayer::StopSprinting()
//{
//	GetCharacterMovement()->MaxWalkSpeed = 300.f;
//}


//Ability System
UAbilitySystemComponent* AUnseenCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

TObjectPtr<class USpringArmComponent> AUnseenCharacterPlayer::GetSpringArmComponent()
{
	return CameraBoom;
}

void AUnseenCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 0);

		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 1);

		EnhancedInputComponent->BindAction(StepBackAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 2);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 3);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 3);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 4);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 4);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 5);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 5);
		
	}
}

void AUnseenCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		// Ability already active
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		// Ability not active
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AUnseenCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		// Ability already active
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AUnseenCharacterPlayer::OnRollMovementTimelineUpdated(float Value)
{
	GetCharacterMovement()->Velocity = Value * RollDistance * GetActorForwardVector();
}

void AUnseenCharacterPlayer::OnStepBackMovementTimelineUpdated(float Value)
{
	GetCharacterMovement()->Velocity = Value * StepBackDistance * GetActorForwardVector();
}

UTimelineComponent* AUnseenCharacterPlayer::GetRollMovementTimeline()
{
	return RollMovementTimeline;
}

UTimelineComponent* AUnseenCharacterPlayer::GetStepBackMovementTimeline()
{
	return StepBackMovementTimeline;
}

void AUnseenCharacterPlayer::RollStepBackCameraLerp()
{
	UpdateCurrentTargetArmLength();
	if (bIsRollStepBackActive)
	{
		RollStepBackCameraTimeline->PlayFromStart();
	}

	else
	{
		RollStepBackCameraTimeline->ReverseFromEnd();
	}
}

void AUnseenCharacterPlayer::AimCameraLerp()
{
	UpdateCurrentTargetArmLength();
	UpdateCurrentSpringArmSocketOffset();

	// To prevent overwriting
	if (RollStepBackCameraTimeline->IsPlaying())
	{
		RollStepBackCameraTimeline->Stop();
	}

	AimCameraTimeline->PlayFromStart();
}

void AUnseenCharacterPlayer::OnRollStepBackCameraTimelineUpdated(float Value)
{
	//float CurrentTargetArmLength = GetSpringArmComponent()->TargetArmLength;

	if (AimCameraTimeline->IsPlaying())
	{
		AimCameraTimeline->Stop();
	}

	if (bIsRollStepBackActive)
	{
		GetSpringArmComponent()->TargetArmLength = FMath::Lerp(CurrentTargetArmLength, 300, Value);
	}
	else
	{
		GetSpringArmComponent()->TargetArmLength = FMath::Lerp(200, CurrentTargetArmLength, Value);
	}

	/*if (bIsRollStepBackActive)
	{
		GetSpringArmComponent()->TargetArmLength = FMath::Max(CurrentTargetArmLength, Value);
	}
	else
	{
		GetSpringArmComponent()->TargetArmLength = FMath::Min(CurrentTargetArmLength, Value);
	}*/
}

void AUnseenCharacterPlayer::OnAimCameraTimelineUpdated(float Value)
{
	// 나중에 값들 변수로 빼자
	if (bIsAiming)
	{
		GetSpringArmComponent()->TargetArmLength = FMath::Lerp(CurrentTargetArmLength, 62, Value);
		GetSpringArmComponent()->SocketOffset = FVector(0, 68, FMath::Lerp(CurrentSpringArmSocketOffset.Z, 56, Value));
		//GetSpringArmComponent()->SocketOffset = FVector(0, FMath::Lerp(CurrentSpringArmSocketOffset.Y, 71, Value), FMath::Lerp(CurrentSpringArmSocketOffset.Z, 56, Value));
	}
	else
	{
		GetSpringArmComponent()->TargetArmLength = FMath::Lerp(CurrentTargetArmLength, 200, Value);
		GetSpringArmComponent()->SocketOffset = FVector(0, 70, FMath::Lerp(CurrentSpringArmSocketOffset.Z, 50, Value));
		//GetSpringArmComponent()->SocketOffset = FVector(0, FMath::Lerp(CurrentSpringArmSocketOffset.Y, 70, Value), FMath::Lerp(CurrentSpringArmSocketOffset.Z, 50, Value));
	}
}

FVector2D AUnseenCharacterPlayer::GetLastInputMovementVector()
{
	return LastInputMovementVector;
}

void AUnseenCharacterPlayer::UpdateCurrentTargetArmLength()
{
	CurrentTargetArmLength = GetSpringArmComponent()->TargetArmLength;
}

void AUnseenCharacterPlayer::UpdateCurrentSpringArmSocketOffset()
{
	CurrentSpringArmSocketOffset = GetSpringArmComponent()->SocketOffset;
}

void AUnseenCharacterPlayer::StopRegenStamina()
{
	if (!bIsBlockedRegenStamina)
	{
		bIsBlockedRegenStamina = true;
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Character.State.BlockedRegenStamina"));
	}
}

void AUnseenCharacterPlayer::StartRegenStaminaWithDelay(float DelayTime)
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = "StartRegenStamina";
	LatentActionInfo.UUID = GetUniqueID();
	LatentActionInfo.Linkage = 0;
	
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), DelayTime, LatentActionInfo);
}

void AUnseenCharacterPlayer::StartRegenStamina()
{
	if (bIsBlockedRegenStamina && !bIsSprinting)
	{
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Character.State.BlockedRegenStamina"));
		bIsBlockedRegenStamina = false;
	}
}
