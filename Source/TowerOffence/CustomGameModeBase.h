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
	UFUNCTION()
	void CheckWinConditions();

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void AddEnemy();

	UFUNCTION()
	void DeleteEnemy();

	UFUNCTION()
	void SetPlayerState(bool SetPlayerState);

	UFUNCTION()
	void SpawnWinWidget();

	UFUNCTION()
	void SpawnLoseWidget();

	UFUNCTION()
	void DestroyWidget();
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TSubclassOf<UUserWidget> WinWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TSubclassOf<UUserWidget> LoseWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	float TimeBeforeRestart = 5.0f;
private:
	bool bIsPlayerAlive = true;
	int EnemiesRemains = 0.0f;
};
