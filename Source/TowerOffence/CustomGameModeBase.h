// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENCE_API ACustomGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void OnPawnCreated(const APawn* Pawn);

	void OnPawnKilled(const APawn* Pawn);

private:
	
	UFUNCTION()
	void CheckWinConditions(const APawn* Pawn);

	UFUNCTION()
	void RestartGame();
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	float TimeBeforeRestart = 5.0f;
	
private:
	bool bIsPlayerAlive = false;
	int EnemiesRemains = 0;
};
