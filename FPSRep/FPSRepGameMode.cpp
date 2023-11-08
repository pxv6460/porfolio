// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSRepGameMode.h"
#include "FPSRepCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSRepGameMode::AFPSRepGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
