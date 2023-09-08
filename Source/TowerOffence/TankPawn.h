// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
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
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Move(const FInputActionValue& InValue);

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Turn(const FInputActionValue& InValue);

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Fire();
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UCameraComponent> Camera;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> MoveForwardAction;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> TurnRightAction;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UInputAction> FireAction;
};
