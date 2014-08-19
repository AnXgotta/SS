// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SS.h"
#include "SSGameMode.h"
#include "SSHUD.h"
#include "SSCharacter.h"
#include "SSPlayerController.h"

ASSGameMode::ASSGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use custom PlayerController class
	PlayerControllerClass = ASSPlayerController::StaticClass();

	// use our custom HUD class
	HUDClass = ASSHUD::StaticClass();
}
