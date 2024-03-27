// Copyright Epic Games, Inc. All Rights Reserved.

#include "MOBA_PrototypeGameMode.h"
#include "MOBA_PrototypePlayerController.h"
#include "MOBA_PrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMOBA_PrototypeGameMode::AMOBA_PrototypeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMOBA_PrototypePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}