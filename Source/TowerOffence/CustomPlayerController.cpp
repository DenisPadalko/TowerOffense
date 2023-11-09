// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"

ACustomPlayerController::ACustomPlayerController()
{
}

void ACustomPlayerController::OnBeginPlay(float DelayTime)
{
	
}

void ACustomPlayerController::SetPlayerEnabledState()
{
	EnableInput(Cast<APlayerController>(this));
}

void ACustomPlayerController::SetPlayerDisabledState()
{
	DisableInput(Cast<APlayerController>(this));
}

void ACustomPlayerController::SpawnWinWidget()
{
	if(WinWidget)
	{
		if(!WidgetInstance)
		{
			WidgetInstance = CreateWidget(GetWorld()->GetFirstPlayerController(), WinWidget);
		}
		if(!WidgetInstance->IsInViewport())
		{
			WidgetInstance->AddToViewport(9999);
		}
	}
}

void ACustomPlayerController::SpawnLoseWidget()
{
	if(LoseWidget)
	{
		if(!WidgetInstance)
		{
			WidgetInstance = CreateWidget(GetWorld()->GetFirstPlayerController(), LoseWidget);
		}
		if(!WidgetInstance->IsInViewport())
		{
			WidgetInstance->AddToViewport(9999);
		}
	}
}

void ACustomPlayerController::SpawnBeforeStartWidget()
{
	if(BeforeStartWidget)
	{
		if(!WidgetInstance)
		{
			WidgetInstance = CreateWidget(GetWorld()->GetFirstPlayerController(), BeforeStartWidget);
		}
		if(!WidgetInstance->IsInViewport())
		{
			WidgetInstance->AddToViewport(9999);
		}
	}
}

void ACustomPlayerController::DestroyWidget()
{
	if(WidgetInstance)
	{
		WidgetInstance->RemoveFromParent();
		WidgetInstance = nullptr;
	}
}