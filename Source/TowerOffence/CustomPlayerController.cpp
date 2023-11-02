// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

#include "GameFramework/PlayerState.h"

ACustomPlayerController::ACustomPlayerController()
{
}

void ACustomPlayerController::SetPlayerEnabledState()
{
	PlayerState->EnableInput(this);
}
