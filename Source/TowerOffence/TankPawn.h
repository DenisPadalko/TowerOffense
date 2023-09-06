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

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Move(float InValue){}

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Turn(float InValue){}

	UFUNCTION(BlueprintCallable, Category="Functions")
	void Fire(){}
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UCameraComponent> Camera;
};
