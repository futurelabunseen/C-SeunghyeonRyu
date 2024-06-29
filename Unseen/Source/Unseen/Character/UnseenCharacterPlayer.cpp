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
#include "UI/UnseenCharacterHUD.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "UI/PauseMenu.h"
#include "UI/GameOver.h"
#include "UI/GameClearUI.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Game/UnseenGameInstance.h"

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

	bIsRollStepBackActive = false;
	bIsBlockedRegenStamina = false;
	bIsSprinting = false;
	bIsShooting = false;
	bIsMoving = false;
	bIsDead = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseUIClassRef(TEXT("/Game/UI/WBP_PauseMenu.WBP_PauseMenu_C"));
	if (PauseUIClassRef.Class)
	{
		PauseUIClass = PauseUIClassRef.Class;
	}
	PauseMenuWidget = nullptr;
	// HUD
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDClassRef(TEXT("/Game/UI/WBP_UnseenPlayerHUD.WBP_UnseenPlayerHUD_C"));
	if (HUDClassRef.Class)
	{
		PlayerHUDClass = HUDClassRef.Class;
	}
	PlayerHUD = nullptr;

	/*static ConstructorHelpers::FClassFinder<UUserWidget> RestartUIClassRef(TEXT("/Game/UI/WBP_GameOverUI.WBP_GameOverUI_C"));
	if (RestartUIClassRef.Class)
	{
		RestartUIClass = RestartUIClassRef.Class;
	}
	RestartUI = nullptr;*/

	// Weapon
	static ConstructorHelpers::FClassFinder<AActor> AssaultRifleBPClassRef(TEXT("/Game/Weapon/USBP_Assault_Rifle.USBP_Assault_Rifle_C"));
	if (AssaultRifleBPClassRef.Class)
	{
		AssaultRifleBPClass = AssaultRifleBPClassRef.Class;
	}
	AssaultRifle = nullptr;
	WeaponOnHand = nullptr;

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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShootRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Shoot.IA_Shoot'"));
	if (nullptr != InputActionShootRef.Object)
	{
		ShootAction = InputActionShootRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Reload.IA_Reload'"));
	if (nullptr != InputActionReloadRef.Object)
	{
		ReloadAction = InputActionReloadRef.Object;
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShootingMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Shooting_Burst_Montage.AM_Shooting_Burst_Montage'"));
	if (ShootingMontageRef.Succeeded())
	{
		ShootingMontage = ShootingMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Reload.AM_Reload'"));
	if (ReloadMontageRef.Succeeded())
	{
		ReloadMontage = ReloadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RifleMainBodyShootMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Weapon/AnimMontage/AM_Shooting_Automatic_Assault_Rifle.AM_Shooting_Automatic_Assault_Rifle'"));
	if (RifleMainBodyShootMontageRef.Succeeded())
	{
		RifleMainBodyShootMontage = RifleMainBodyShootMontageRef.Object;
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

	static ConstructorHelpers::FClassFinder<AActor> AssaultRifleProjectileBPClassRef(TEXT("/Script/Engine.Blueprint'/Game/Weapon/USBP_Projectile_AssaultRifle.USBP_Projectile_AssaultRifle_C'"));
	if (AssaultRifleProjectileBPClassRef.Class)
	{
		AssaultRifleProjectileBPClass = AssaultRifleProjectileBPClassRef.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> GameOverSoundObject(TEXT("/Script/Engine.SoundCue'/Game/Sound/gameover_Cue.gameover_Cue'"));
	if (nullptr != GameOverSoundObject.Object)
	{
		GameOverSound = GameOverSoundObject.Object;
	}

	RollMovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RollMovementTimeline"));
	RollDistance = 400.f;

	StepBackMovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("StepBackMovementTimeline"));
	StepBackDistance = -650.f; // not real distance.

	RollStepBackCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RollStepBackCameraTimeline"));

	AimCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimCameraTimeline"));

	CharacterMaxAmmo = 999;
	CharacterCurrentAmmo = 150;

	bApplyRecoil = true;

	tempVal = 600.0f;
	StartMoney = 1000.0f;
	Money = StartMoney;
	MaxMoney = 100000.0f;
	ShootRateMoney = 500.0f;
	VerticalRecoilMoney = 500.0f;
	HorizontalRecoilMoney = 500.0f;
	BulletMoney = 500.0f;

}

void AUnseenCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AUnseenPlayerState* UnseenPlayerState = GetPlayerState<AUnseenPlayerState>();
	if (UnseenPlayerState)
	{
		ASC = UnseenPlayerState->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(UnseenPlayerState, this);
		AttributeSet = ASC->GetSet<UUnseenCharacterAttributeSet>();
		
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
	
		const_cast<UUnseenCharacterAttributeSet*>(AttributeSet)->OnDie.AddDynamic(this, &AUnseenCharacterPlayer::OnDieCallback);
	}

	//디버깅
	/*APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));*/

	const UGameplayEffect* RegenStaminaEffectCDO = RegenStaminaEffect->GetDefaultObject<UGameplayEffect>();
	ASC->ApplyGameplayEffectToSelf(RegenStaminaEffectCDO, 0, ASC->MakeEffectContext());

}

void AUnseenCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	/*if (Controller == nullptr)
	{
		Controller = GetWorld()->GetFirstPlayerController();
		Controller->Possess(this);
	}*/
	// Only for Player, So use CastChecked
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
	}

	if (PauseUIClass)
	{
		PauseMenuWidget = CreateWidget<UPauseMenu>(PlayerController, PauseUIClass);
		if (nullptr != PauseMenuWidget)
		{
			PauseMenuWidget->AddToViewport();
			PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (PlayerHUDClass) // 멀티하면 IsLocallyControlled() 찾아보기
	{
		PlayerHUD = CreateWidget<UUnseenCharacterHUD>(PlayerController, PlayerHUDClass);
		if (nullptr != PlayerHUD)
		{
			PlayerHUD->AddToPlayerScreen();
		}
	}

	if (AssaultRifleBPClass)
	{
		AssaultRifle = GetWorld()->SpawnActor<AAssaultRifle>(AssaultRifleBPClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (nullptr != AssaultRifle)
		{
			AssaultRifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RH_Rifle"));
			WeaponOnHand = AssaultRifle;
			WeaponOnHand->bIsOnHand = true;
			if (PlayerHUD)
			{
				WeaponOnHand->OwnerPlayerHUD = PlayerHUD;
			}
		}
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

	if (RifleMainBodyShootMontage)
	{
		const auto NotifyEvents = RifleMainBodyShootMontage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto ShootStartNotify = Cast<UAnimNotify_Shoot>(EventNotify.Notify))
			{
				ShootStartNotify->OnNotified.AddUObject(this, &AUnseenCharacterPlayer::ShootWeapon);
			}

			if (const auto ShootEndNotify = Cast<UAnimNotify_ShootEnd>(EventNotify.Notify))
			{
				ShootEndNotify->OnNotified.AddUObject(this, &AUnseenCharacterPlayer::ShootWeaponEnd);
			}
		}
	}

	if (ReloadMontage)
	{
		const auto NotifyEvents = ReloadMontage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto ReloadEndNotify = Cast<UAnimNotify_ReloadEnd>(EventNotify.Notify))
			{
				ReloadEndNotify->OnNotified.AddUObject(this, &AUnseenCharacterPlayer::ReloadMontageEnd);
			}
		}
	}

	RespawnCharacterSet();
	ASC->SetNumericAttributeBase(AttributeSet->GetHpAttribute(), AttributeSet->GetMaxHp());
	ASC->SetNumericAttributeBase(AttributeSet->GetStaminaAttribute(), AttributeSet->GetMaxStamina());
	ASC->SetNumericAttributeBase(AttributeSet->GetShootRateAttribute(), WeaponOnHand->ShootRate);
}

//void AUnseenCharacterPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	if (PlayerHUD)
//	{
//		PlayerHUD->RemoveFromParent();
//		PlayerHUD = nullptr;
//	}
//
//	Super::EndPlay(EndPlayReason);
//}

void AUnseenCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector Forward = GetActorForwardVector();
	FVector Start = GetActorLocation();
	Start.Z = 0.f;
	FVector RollDirection = (FVector(CalculateRollDirection().X, CalculateRollDirection().Y, 0.f)).GetSafeNormal();

	/*DrawDebugDirectionalArrow(GetWorld(), Start, Start + Forward * 100.f,
		5.f, FColor::Red, false, -1.f, 0, 2.f);

	DrawDebugDirectionalArrow(GetWorld(), Start, Start + RollDirection * 100.f,
		5.f, FColor::Yellow, false, -1.f, 0, 2.f);*/

}

void AUnseenCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::Look);
	}
}

void AUnseenCharacterPlayer::Move(const FInputActionValue& Value)
{
	LastInputMoveValue = Value.Get<FVector2D>();
}

void AUnseenCharacterPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (nullptr != Controller)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

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

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 1);

		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 2);

		EnhancedInputComponent->BindAction(StepBackAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 3);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 4);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 4);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 5);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 5);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 6);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 6);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 7);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AUnseenCharacterPlayer::GASInputReleased, 7);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AUnseenCharacterPlayer::GASInputPressed, 8);
		
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

FVector2D AUnseenCharacterPlayer::GetLastInputMoveValue()
{
	return LastInputMoveValue;
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

FVector AUnseenCharacterPlayer::CalculateRollDirection()
{
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	return (ForwardDirection * LastInputMoveValue.X) + (RightDirection * LastInputMoveValue.Y);

}

AUnseenWeaponBase* AUnseenCharacterPlayer::GetWeaponOnHand()
{
	if (nullptr != WeaponOnHand)
	{
		return WeaponOnHand;
	}

	UE_LOG(LogTemp, Warning, TEXT("No Weapon On-Hand"));

	return nullptr;
}

void AUnseenCharacterPlayer::ShootWeapon()
{
	if (bIsShooting)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponStart Notify"));
		
		ShootProjectile();
		
		if (bApplyRecoil)
		{
			GetWeaponOnHand()->ShootWeapon();
		}
		else
		{
			GetWeaponOnHand()->ShootWeaponNotHorizontalRecoil();
		}

		if (GetWeaponOnHand()->CurrentAmmo == 0)
		{
			ASC->CancelAbility(ASC->FindAbilitySpecFromInputID(7)->Ability);
		}
	}
}

void AUnseenCharacterPlayer::ShootWeaponEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("WeaponEnd Notify"));
	if (!bIsShooting)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, ShootingMontage);
		GetWeaponOnHand()->ShootingStop();
	}
}

void AUnseenCharacterPlayer::ReloadMontageEnd()
{
	int LeftWeaponAmmo = GetWeaponOnHand()->CurrentAmmo;
	int NeedAmmo = GetWeaponOnHand()->MaxAmmo - LeftWeaponAmmo;
	int ReloadCnt = CharacterCurrentAmmo < NeedAmmo ? CharacterCurrentAmmo : NeedAmmo;
	CharacterCurrentAmmo -= ReloadCnt;
	GetWeaponOnHand()->CurrentAmmo += ReloadCnt;
	GetWeaponOnHand()->ChangeMaterialBulletVariable();
	ASC->CancelAbility(ASC->FindAbilitySpecFromInputID(8)->Ability);
}

bool AUnseenCharacterPlayer::IsCanReload()
{
	return (CharacterCurrentAmmo > 0 && !GetWeaponOnHand()->IsAmmoFull());
}

void AUnseenCharacterPlayer::OnDieCallback()
{
	if (!bIsDead)
	{
		bIsDead = true;
		UE_LOG(LogTemp, Warning, TEXT("Die"));
		UGameplayStatics::PlaySound2D(this, GameOverSound);

		
		GetSpringArmComponent()->TargetArmLength = 500;
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
		PlayerController->DisableInput(PlayerController);
		InputComponent->Deactivate();
		ASC->CancelAllAbilities();
		ASC->ClearAllAbilities();
		GetCharacterMovement()->DisableMovement();
		GetMesh()->SetSimulatePhysics(true);
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);


		if (RestartUIClass)
		{
			RestartUI = CreateWidget<UGameOver>(PlayerController, RestartUIClass);
			if (nullptr != RestartUI)
			{
				RestartUI->AddToViewport();
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->SetShowMouseCursor(true);
			}
		}
	}
}

void AUnseenCharacterPlayer::OnClearCallback()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	PlayerController->DisableInput(PlayerController);
	InputComponent->Deactivate();
	ASC->CancelAllAbilities();
	ASC->ClearAllAbilities();
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetSimulatePhysics(false);
	PlayerController->SetIgnoreMoveInput(true);
	PlayerController->SetIgnoreLookInput(true);

	if (ClearUIClass)
	{
		ClearUI = CreateWidget<UGameClearUI>(PlayerController, ClearUIClass);
		if (nullptr != ClearUI)
		{
			ClearUI->AddToViewport();
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

void AUnseenCharacterPlayer::RespawnCharacterSet()
{
	UUnseenGameInstance* GI = CastChecked<UUnseenGameInstance>(GetWorld()->GetGameInstance());

	if (GI->CurAmmo + GI->WeaponCurAmmo < 30)
	{
		GI->CurAmmo = 0;
		GI->WeaponCurAmmo = 30;
	}

	CharacterCurrentAmmo = GI->CurAmmo;
	Money = GI->CurMoney;
	ShootRateMoney = GI->SRCost;
	VerticalRecoilMoney = GI->VRCost;
	HorizontalRecoilMoney = GI->HRCost;
	BulletMoney = GI->BulletPrice;
	WeaponOnHand->RespawnCharacterSet(GI->WeaponCurAmmo, GI->ShootRate, GI->CVR, GI->CHR, GI->HRA);
	PauseMenuWidget->ShootRateCnt = GI->SRCnt;
	PauseMenuWidget->VerticalCnt = GI->VRCnt;
	PauseMenuWidget->HorizontalCnt = GI->HRCnt;
	PauseMenuWidget->InitialUI();
	
}

void AUnseenCharacterPlayer::ChangeShootRate(float ShootRate)
{
	ASC->SetNumericAttributeBase(AttributeSet->GetShootRateAttribute(), ShootRate);
	GetWeaponOnHand()->ShootRate = ShootRate;
}

void AUnseenCharacterPlayer::IncreaseShootRate(float ShootRate)
{
	float CurrentValue = AttributeSet->GetShootRate();
	ASC->SetNumericAttributeBase(AttributeSet->GetShootRateAttribute(), CurrentValue + ShootRate);
	GetWeaponOnHand()->ShootRate = CurrentValue + ShootRate;
}

void AUnseenCharacterPlayer::DecreaseVerticalRecoil(float value)
{
	GetWeaponOnHand()->CurrentVerticalRecoil -= value;
}

void AUnseenCharacterPlayer::DecreaseHorizontalRecoil(float value)
{
	GetWeaponOnHand()->HorizontalRecoilAmount -= value;
}

bool AUnseenCharacterPlayer::UseMoney(float value)
{
	if (Money >= value)
	{
		Money -= value;
		return true;
	}
	else
	{
		return false;
	}
}

bool AUnseenCharacterPlayer::CanIncraseBullet(int value)
{
	int temp = CharacterCurrentAmmo + value;

	if (temp <= CharacterMaxAmmo)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AUnseenCharacterPlayer::IncraseBullet(int value)
{
	CharacterCurrentAmmo += value;
}

void AUnseenCharacterPlayer::ShootProjectile()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(FName("CameraTrace"),false, this);

	APlayerCameraManager* PlayerCamera = CastChecked<APlayerController>(GetController())->PlayerCameraManager;
	
	//float HorizontalRecoilRandomVar = GetWeaponOnHand()->CurrentHorizontalRecoil; // 일단 최대 recoil 값으로 테스트하면서 적당한 value값 찾기
	float HorizontalRecoilRandomVar = UKismetMathLibrary::RandomFloatInRange(-GetWeaponOnHand()->CurrentHorizontalRecoil, GetWeaponOnHand()->CurrentHorizontalRecoil);
	
	FVector Start = PlayerCamera->GetCameraLocation();
	FVector End = PlayerCamera->GetCameraLocation() + (PlayerCamera->GetActorForwardVector() * 10000.0f);
	bool HitDetected = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 0.2f);

	FVector SpawnLocation = GetWeaponOnHand()->GetMuzzlePos();
	FRotator SpawnRotator;

	if (HitDetected)
	{
		float ProportionalValue = HitResult.Distance * HorizontalRecoilRandomVar / tempVal; // tempVal is experiential value 600.0f is best
		UE_LOG(LogTemp, Warning, TEXT("Distance : %f"), HitResult.Distance);
		FVector ProportionalVector = PlayerCamera->GetActorRightVector() * ProportionalValue;
		SpawnRotator = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, HitResult.ImpactPoint + ProportionalVector);
		//DrawDebugLine(GetWorld(), SpawnLocation, HitResult.ImpactPoint + ProportionalVector, FColor::Blue, true, 1.0f, 0, 0.2f);
	}
	else
	{
		float ProportionalValue = 10000.0f * HorizontalRecoilRandomVar / tempVal;
		FVector ProportionalVector = PlayerCamera->GetActorRightVector() * ProportionalValue;
		SpawnRotator = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, HitResult.TraceEnd + ProportionalVector);
		//DrawDebugLine(GetWorld(), SpawnLocation, HitResult.TraceEnd + ProportionalVector, FColor::Green, true, 1.0f, 0, 0.2f);
	}

	// Object Polling
	if (GetWeaponOnHand()->ProjectilePool != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile pool : %d"), GetWeaponOnHand()->ProjectilePool->Pool.Num());
		AUS_ProjectileBase* Bullet = GetWeaponOnHand()->ProjectilePool->PullProjectile();
		// 데미지 강화 Bullet->CurrentDamage = Bullet->BaseDamage + DamageIncrease;
		Bullet->SetActorLocation(SpawnLocation);
		Bullet->SetActorRotation(SpawnRotator);
		Bullet->SetProjectileActive(true);
		Bullet->SetLifeSpanToPool();
	}

	// Not Using Object Polling
	//GetWorld()->SpawnActor<AUS_Projectile_AssaultRifle>(AssaultRifleProjectileBPClass, SpawnLocation, SpawnRotator);
}
