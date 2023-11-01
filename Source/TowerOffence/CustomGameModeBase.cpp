// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameModeBase.h"

#include "Kismet/GameplayStatics.h"

void ACustomGameModeBase::CheckWinConditions()
{
	if(!bIsPlayerAlive)
	{
		SpawnLoseWidget();
	}
	else if(EnemiesRemains == 0)
	{
		SpawnWinWidget();
	}
	RestartGame();
}

void ACustomGameModeBase::RestartGame()
{
	const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if(PlayerController->IsInputKeyDown(EKeys::SpaceBar))
	{
		DestroyWidget();
		UGameplayStatics::OpenLevel(GetLevel(), "Game level");
	}
}

void ACustomGameModeBase::AddEnemy()
{
	++EnemiesRemains;
}

void ACustomGameModeBase::DeleteEnemy()
{
	--EnemiesRemains;
}


void ACustomGameModeBase::SetPlayerState(bool PlayerState)
{
	bIsPlayerAlive = PlayerState;
}


void ACustomGameModeBase::SpawnWinWidget()
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

void ACustomGameModeBase::SpawnLoseWidget()
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

void ACustomGameModeBase::DestroyWidget()
{
	if(WidgetInstance)
	{
		WidgetInstance->RemoveFromParent();
		WidgetInstance = nullptr;
	}
}
