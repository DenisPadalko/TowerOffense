// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DELEGATE(OnDamageTakenDelegate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWEROFFENCE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetCurrentHealth() const;

	float DecreaseHealth(const float DecreaseValue);

	bool IsZero() const;

	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float DamageTaken, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
public:
	UPROPERTY(EditAnywhere, Category="Default")
	float DefaultHealth = 100.0f;

	float CurrentHealth = DefaultHealth;

	OnDamageTakenDelegate OnDamageTaken;
};
