// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENCE_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACustomPlayerController();
	
	UFUNCTION()
	void SetPlayerEnabledState();

	UFUNCTION()
	void SpawnWinWidget();

	UFUNCTION()
	void SpawnLoseWidget();

	UFUNCTION()
	void DestroyWidget();
	
private:

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TSubclassOf<UUserWidget> WinWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TSubclassOf<UUserWidget> LoseWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance = nullptr;
};
