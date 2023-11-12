// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerController.h"
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

	void OnBeginPlay();

private:
	
	UFUNCTION()
	void CheckWinConditions(const APawn* Pawn);

	UFUNCTION()
	void RestartGame();

	void CreateStartWidget() const;

	void DestroyStartWidget() const;
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	float TimeBeforeRestart = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Default")
	float DelayBeforeStart = 5.0f;
	
private:
	bool bIsPlayerAlive = false;
	int EnemiesRemains = 0;
};
