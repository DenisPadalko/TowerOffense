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

	void TurnTurret(const FRotator& InValue);

	TArray<FString> GetProjectileNameOptions() const;

	UFUNCTION()
	void Fire();

	UFUNCTION()
	virtual void CheckHealth(float CurrentHealth);

	UFUNCTION()
	virtual void ShakeCamera();
	
	void SpawnShootParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnHitParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnDeathParticle(const FVector& Location, const FRotator& Rotation) const;

	void SpawnShootSound(const FVector& Location, const FRotator& Rotation) const;

	void SpawnHitSound(const FVector& Location, const FRotator& Rotation) const;

	void SpawnOnDeathSound(const FVector& Location, const FRotator& Rotation) const;

	void SpawnTurretTurningSound(const FVector& Location, const FRotator& Rotation);

	void SpawnAmbientSound(const FVector& Location, const FRotator& Rotation);

	void DestroyTurretTurningSound();

	void DestroyAmbientSound();

	bool IsTurretTurningSoundSpawned() const;
	
	TSubclassOf<UCameraShakeBase> GetCameraShake() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
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
	TObjectPtr<USoundBase> AmbientSound = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY()
	TObjectPtr<AProjectile> Projectile = nullptr;
private:
	
	TObjectPtr<UAudioComponent> TurretTurningSoundComponent = nullptr;
	TObjectPtr<UAudioComponent> AmbientSoundComponent = nullptr;
};
