// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

DECLARE_DELEGATE_OneParam(OnTargetHitDelegate, AActor*)

UCLASS()
class TOWEROFFENCE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Destroy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		FVector Impulse, const FHitResult& Hit);

public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Default")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent = nullptr;

	UPROPERTY(EditAnywhere, Category="Default")
	float SpeedOfMovement = 1.0f;

	UPROPERTY(EditAnywhere, Category="Default")
	float Damage = 10.0f;

	OnTargetHitDelegate OnTargetHit;
};