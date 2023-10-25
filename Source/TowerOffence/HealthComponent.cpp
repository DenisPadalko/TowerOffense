// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<AActor> Owner = GetOwner();
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamage);
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::DecreaseHealth(const float DecreaseValue)
{
	CurrentHealth -= DecreaseValue;
	if(CurrentHealth < 0.0f)
	{
		CurrentHealth = 0.0f;
	}
	return CurrentHealth;
}

bool UHealthComponent::IsZero() const
{
	return FMath::IsNearlyZero(CurrentHealth);
}

void UHealthComponent::OnDamage(AActor* DamagedActor, float DamageTaken, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	DecreaseHealth(DamageTaken);

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), CurrentHealth);
	OnDamageTaken.Execute();
}
