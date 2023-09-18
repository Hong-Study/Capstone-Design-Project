// Copyright Epic Games, Inc. All Rights Reserved.


#include "S1GameModeBase.h"
#include "Player/MyPlayerController.h"

AS1GameModeBase::AS1GameModeBase()
{
	PlayerControllerClass = AMyPlayerController::StaticClass();
}