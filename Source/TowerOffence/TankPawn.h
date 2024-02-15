// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "TurretPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankPawn.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENCE_API ATankPawn : public ATurretPawn
{
	GENERATED_BODY()
public:
	ATankPawn();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="Functions")
	void InputMove(const FInputActionValue& InValue);

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Turn(const FInputActionValue& InValue);

	UFUNCTION(BlueprintCallable, Category="Functions")
	void CallFire();

	virtual void CheckHealth(float CurrentHealth) override;

	void SpawnDustFromTank(const TArray<TObjectPtr<USceneComponent>> AttachToComponent);

	void SpawnLeftDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent);

	void SpawnRightDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent);
	
	void SpawnMovementSound(const FVector& Location, const FRotator& Rotation);
	
	void DestroyDustFromTank();
	
	void DestroyMovementSound();
	
	bool IsMovementSoundSpawned() const;

	void ShakeCamera();

	UFUNCTION()
	void IsHitActorDead(AActor* HitActor);
	
private:
	void Move(const float Direction);
	
	float GetCurrentSpeed() const;
	
	void FinishMoving();

	void FinishTurn();
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USceneComponent> LeftDustSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USceneComponent> RightDustSpawnPoint;

	UPROPERTY(EditAnywhere, Category="Default")
	float MovementSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category="Default")
	float RotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Default")
	float AccelerationDuration = 5.0f;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> MoveForwardAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> TurnRightAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> FireAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundBase> MovementSound = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TSubclassOf<UCameraShakeBase> CameraShake;
	
private:
	float MovementTime = 0.0f;
	
	TObjectPtr<UParticleSystemComponent> LeftDustFromTankComponent = nullptr;
	TObjectPtr<UParticleSystemComponent> RightDustFromTankComponent = nullptr;
	
	TObjectPtr<UAudioComponent> MovementSoundComponent = nullptr;
};
