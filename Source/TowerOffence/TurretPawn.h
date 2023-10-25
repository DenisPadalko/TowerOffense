// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

UCLASS()
class TOWEROFFENCE_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurretPawn();

	UFUNCTION(CallInEditor)
	TArray<FString> GetNameOptions() const;

	virtual void PostInitializeComponents() override;

	void TurnTurret(const FRotator& InValue) const;

	TArray<FString> GetProjectileNameOptions() const;

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void CheckHealth();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Restart();

	void CheckWinConditions();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UStaticMeshComponent> TurretMesh;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TSubclassOf<AProjectile> ProjectileToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="Default", meta=(GetOptions = "GetNameOptions"))
	FName MaterialSlotName = "TeamColor";

	UPROPERTY(EditDefaultsOnly, Category="Default")
	FName ParameterName = "Color";

	UPROPERTY(EditDefaultsOnly, Category="Default")
	FColor ColorOfTeam = FColor::Yellow;

	UPROPERTY(EditAnywhere, Category="Default")
	float TurretRotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Default")
	float TimeBetweenShots = 5.0f;

	float TimeAfterLastShot = 1.0f;
};
