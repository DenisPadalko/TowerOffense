// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameModeBase.h"

#include "CustomPlayerController.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void ACustomGameModeBase::OnBeginPlay() 
{
	Super::BeginPlay();

	CreateStartWidget();
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACustomGameModeBase::DestroyStartWidget, DelayBeforeStart);
}

void ACustomGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DestroyAmbientSound();
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

void ACustomGameModeBase::SpawnDustFromTank(const TArray<TObjectPtr<USceneComponent>> AttachToComponent)
{
	if(LeftDustFromTankComponent || RightDustFromTankComponent)
	{
		return;
	}
	SpawnLeftDustFromTankComponent(AttachToComponent[0]);
	SpawnRightDustFromTankComponent(AttachToComponent[1]);
}

void ACustomGameModeBase::SpawnLeftDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent)
{
	LeftDustFromTankComponent = UGameplayStatics::SpawnEmitterAttached(DustFromTank, AttachToComponent, FName("Dust point"),
		FVector(ForceInit), FRotator::ZeroRotator, FVector(1), EAttachLocation::KeepRelativeOffset,
		false, EPSCPoolMethod::None, false);
	LeftDustFromTankComponent->Activate(true);
}

void ACustomGameModeBase::SpawnRightDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent)
{
	RightDustFromTankComponent = UGameplayStatics::SpawnEmitterAttached(DustFromTank, AttachToComponent, FName("Dust point"),
		FVector(ForceInit), FRotator::ZeroRotator, FVector(1), EAttachLocation::KeepRelativeOffset,
		false, EPSCPoolMethod::None, false);
	RightDustFromTankComponent->Activate(true);
}

void ACustomGameModeBase::SpawnHitSound(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Location, Rotation);
}

void ACustomGameModeBase::SpawnShootSound(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound, Location, Rotation);
}

void ACustomGameModeBase::SpawnOnDeathSound(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), OnDeathSound, Location, Rotation);
}

void ACustomGameModeBase::SpawnTurretTurningSound(const FVector& Location, const FRotator& Rotation)
{
	TurretTurningSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), TurretTurningSound, Location, Rotation);
}

void ACustomGameModeBase::SpawnMovementSound(const FVector& Location, const FRotator& Rotation)
{
	MovementSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), MovementSound, Location, Rotation);
}

void ACustomGameModeBase::SpawnAmbientSound(const FVector& Location, const FRotator& Rotation)
{
	AmbientSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AmbientSound, Location, Rotation);
}

void ACustomGameModeBase::DestroyTurretTurningSound()
{
	TurretTurningSoundComponent->Deactivate();
	TurretTurningSoundComponent = nullptr;
}

void ACustomGameModeBase::DestroyMovementSound()
{
	MovementSoundComponent->Deactivate();
	MovementSoundComponent = nullptr;
}

void ACustomGameModeBase::DestroyAmbientSound()
{
	AmbientSoundComponent->Deactivate();
	AmbientSoundComponent = nullptr;
}

void ACustomGameModeBase::DestroyDustFromTank()
{
	if(LeftDustFromTankComponent)
	{
		LeftDustFromTankComponent->Deactivate();
		LeftDustFromTankComponent = nullptr;
	}
	if(RightDustFromTankComponent)
	{
		RightDustFromTankComponent->Deactivate();
		RightDustFromTankComponent = nullptr;
	}
}

bool ACustomGameModeBase::IsTurretTurningSoundSpawned() const
{
	return TurretTurningSoundComponent != nullptr;
}

bool ACustomGameModeBase::IsMovementSoundSpawned() const
{
	return MovementSoundComponent != nullptr;
}
