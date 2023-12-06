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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SpawnShootParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnHitParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnDeathParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnDustFromTank(const TArray<TObjectPtr<USceneComponent>> AttachToComponent);

	void SpawnLeftDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent);

	void SpawnRightDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent);

	void SpawnShootSound(const FVector& Location, const FRotator& Rotation) const;

	void SpawnHitSound(const FVector& Location, const FRotator& Rotation) const;

	void SpawnOnDeathSound(const FVector& Location, const FRotator& Rotation) const;

	void SpawnTurretTurningSound(const FVector& Location, const FRotator& Rotation);

	void SpawnMovementSound(const FVector& Location, const FRotator& Rotation);

	void SpawnAmbientSound(const FVector& Location, const FRotator& Rotation);

	void DestroyTurretTurningSound();

	void DestroyMovementSound();

	void DestroyAmbientSound();
	
	void DestroyDustFromTank();

	bool IsTurretTurningSoundSpawned() const;

	bool IsMovementSoundSpawned() const;

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

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundBase> ShootSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundBase> HitSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundBase> OnDeathSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundBase> TurretTurningSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundBase> MovementSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundBase> AmbientSound = nullptr;
private:
	bool bIsPlayerAlive = false;
	int EnemiesRemains = 0;

	TObjectPtr<UParticleSystemComponent> LeftDustFromTankComponent = nullptr;
	TObjectPtr<UParticleSystemComponent> RightDustFromTankComponent = nullptr;

	TObjectPtr<UAudioComponent> TurretTurningSoundComponent = nullptr;
	TObjectPtr<UAudioComponent> MovementSoundComponent = nullptr;
	TObjectPtr<UAudioComponent> AmbientSoundComponent = nullptr;
};
