// Copyright Epic Games, Inc. All Rights Reserved.


#include "UnseenGameModeBase.h"

AUnseenGameModeBase::AUnseenGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/Characters/GASUnseen/BP_UnseenCharacterPlayer"));
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Script/Unseen.UnseenCharacterPlayer"));
	if (ThirdPersonClassRef.Class != nullptr)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Unseen.UnseenPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateClassRef(TEXT("/Script/Unseen.UnseenPlayerState"));
	if (PlayerStateClassRef.Class)
	{
		PlayerStateClass = PlayerStateClassRef.Class;
	}

}
