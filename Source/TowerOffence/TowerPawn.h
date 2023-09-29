// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretPawn.h"
#include "Components/SphereComponent.h"
#include "Engine/TriggerSphere.h"
#include "TowerPawn.generated.h"

/**
 * 
 */
UCLASS()
class TOWEROFFENCE_API ATowerPawn : public ATurretPawn
{
	GENERATED_BODY()
public:
	ATowerPawn();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	void SetCollisionSphereRadius() const;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);
private:
	void Fire();

	void GetRotation(const TObjectPtr<AActor> Player, FRotator* Rotation) const;

	TObjectPtr<AActor> GetClosestTarget() const;
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<USphereComponent> CollisionSphere = nullptr;

	UPROPERTY(EditAnywhere, Category="Default")
	float CollisionSphereRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category="Default")
	float TimeBetweenShots = 5.0f;

	UPROPERTY(EditAnywhere, Category="Default")
	float RotationTolerance = 5.0f;
private:
	float TimeAfterLastShot = 1.0f;
	
	TSet<TObjectPtr<AActor>> PlayerRef;
};
