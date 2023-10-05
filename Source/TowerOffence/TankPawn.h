// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UFUNCTION(BlueprintCallable, Category="Functions")
	void InputMove(const FInputActionValue& InValue);

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Turn(const FInputActionValue& InValue);

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Fire();

	void Move(const float Direction);
	float GetCurrentSpeed() const;
	void FinishMoving();
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category="Default")
	float MovementSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category="Default")
	float RotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Default")
	float AccelerationDuration = 5.0f;

	UPROPERTY(EditAnywhere, Category="Default", meta=(GetOptions="BP_Projectile"))
	TSubclassOf<AActor> ProjectileToSpawn;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> MoveForwardAction;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> TurnRightAction;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> FireAction;
private:
	float MovementTime = 0.0f;
};
