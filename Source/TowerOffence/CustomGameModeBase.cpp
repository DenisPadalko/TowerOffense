// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameModeBase.h"

#include "CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void ACustomGameModeBase::OnBeginPlay() 
{
	Super::BeginPlay();

	CreateStartWidget();
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACustomGameModeBase::DestroyStartWidget, DelayBeforeStart);
}

void ACustomGameModeBase::OnPawnCreated(const APawn* Pawn)
{
	if(Pawn == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		bIsPlayerAlive = true;
	}
	else
	{
		++EnemiesRemains;
	}
}

void ACustomGameModeBase::OnPawnKilled(const APawn* Pawn)
{
	CheckWinConditions(Pawn);
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACustomGameModeBase::RestartGame, TimeBeforeRestart);
}

void ACustomGameModeBase::CheckWinConditions(const APawn* Pawn)
{
	const TObjectPtr<ACustomPlayerController> PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	
	if(Pawn->IsPawnControlled())
	{
		bIsPlayerAlive = false;
		PlayerController->SpawnLoseWidget();
	}
	else
	{
		--EnemiesRemains;
	}
	if(EnemiesRemains == 0)
	{
		PlayerController->SpawnWinWidget();
	}
}

void ACustomGameModeBase::RestartGame()
{
	const TObjectPtr<ACustomPlayerController> PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	
	PlayerController->DestroyWidget();
	UGameplayStatics::OpenLevel(GetLevel(), "Game level");
}

void ACustomGameModeBase::CreateStartWidget() const
{
	const TObjectPtr<ACustomPlayerController> PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	
	PlayerController->SetPlayerDisabledState();
	PlayerController->SpawnBeforeStartWidget();
}

void ACustomGameModeBase::DestroyStartWidget() const
{
	const TObjectPtr<ACustomPlayerController> PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());

	PlayerController->DestroyWidget();
	PlayerController->SetPlayerEnabledState();
}

void ACustomGameModeBase::SpawnShootParticle(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootParticle, Location, Rotation);
}

void ACustomGameModeBase::SpawnHitParticle(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Location, Rotation);
}

void ACustomGameModeBase::SpawnDeathParticle(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, Location, Rotation);
}
/*
void ACustomGameModeBase::SpawnDustFromTank(const TObjectPtr<USceneComponent> AttachToComponent)
{
	//FVector SpawnLocation = Location;
	//SpawnLocation.Z -= 25.0f;
	//DustFromTankComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DustFromTank, SpawnLocation, Rotation);

	//FTimerHandle UnusedHandle;
	//GetWorldTimerManager().SetTimer(UnusedHandle, this,  &ACustomGameModeBase::DestroyDustFromTank, 1.0f);

	DustFromTankComponent = UGameplayStatics::SpawnEmitterAttached(DustFromTank, AttachToComponent);
}

void ACustomGameModeBase::DestroyDustFromTank()
{
	DustFromTankComponent->DeactivateSystem();
	DustFromTankComponent->DestroyComponent();
}
*/