// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DELEGATE_OneParam(OnDamageTakenDelegate, float)

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
	float GetCurrentHealth() const;

	float ChangeHealth(const float ChangeValue);

	bool IsZero() const;

	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float DamageTaken, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
public:
	UPROPERTY(EditAnywhere, Category="Default")
	float MaxHealth = 100.0f;

	float CurrentHealth = MaxHealth;

	OnDamageTakenDelegate OnDamageTaken;
};
