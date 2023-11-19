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

	void SpawnShootParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnHitParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnDeathParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnDustFromTank(const TArray<TObjectPtr<USceneComponent>> AttachToComponent);

	void SpawnLeftDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent);

	void SpawnRightDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent);

	void DestroyDustFromTank();

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

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UParticleSystem> ShootParticle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UParticleSystem> HitParticle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UParticleSystem> DeathParticle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UParticleSystem> DustFromTank = nullptr;
	
private:
	bool bIsPlayerAlive = false;
	int EnemiesRemains = 0;

	TObjectPtr<UParticleSystemComponent> LeftDustFromTankComponent = nullptr;
	TObjectPtr<UParticleSystemComponent> RightDustFromTankComponent = nullptr;
};
