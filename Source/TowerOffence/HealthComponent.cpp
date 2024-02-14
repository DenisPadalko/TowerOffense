// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<AActor> Owner = GetOwner();
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamage);
	// ...
	
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::ChangeHealth(const float ChangeValue)
{
	CurrentHealth += ChangeValue;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	return CurrentHealth;
}

bool UHealthComponent::IsZero() const
{
	return FMath::IsNearlyZero(CurrentHealth);
}

void UHealthComponent::OnDamage(AActor* DamagedActor, float DamageTaken, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	ChangeHealth(DamageTaken);

	OnDamageTaken.Execute(CurrentHealth);
}
