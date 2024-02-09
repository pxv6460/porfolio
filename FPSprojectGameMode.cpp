// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSprojectGameMode.h"
#include "FPSprojectHUD.h"
#include "FPSprojectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSprojectGameMode::AFPSprojectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSprojectHUD::StaticClass();
}
