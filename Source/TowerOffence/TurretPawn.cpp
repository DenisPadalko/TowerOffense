// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"

#include "Components/AudioComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule component"));
	SetRootComponent(CapsuleComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret mesh"));
	TurretMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile spawn point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDamageTaken.BindUObject(this, &ATurretPawn::CheckHealth);
}

void ATurretPawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	SpawnHitParticle(HitLocation, FRotator::ZeroRotator);
	SpawnHitSound(HitLocation, FRotator::ZeroRotator);
}

TArray<FString> ATurretPawn::GetNameOptions() const
{
	TArray<FName> MaterialSlotNames = TurretMesh->GetMaterialSlotNames();
	TArray<FString> Result;
	
	for(const FName& Name : MaterialSlotNames)
	{
		Result.Add(Name.ToString());
	}
	
	return Result;
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();

	SpawnAmbientSound(GetActorLocation(), GetActorRotation());
}

// Called every frame
void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurretPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	const int MaterialIndex = TurretMesh->GetMaterialIndex(MaterialSlotName);
	TurretMesh->CreateDynamicMaterialInstance(MaterialIndex);
	TurretMesh->SetVectorParameterValueOnMaterials(ParameterName, FVector(ColorOfTeam));
	BaseMesh->CreateDynamicMaterialInstance(BaseMesh->GetMaterialIndex(MaterialSlotName));
	BaseMesh->SetVectorParameterValueOnMaterials(ParameterName, FVector(ColorOfTeam));
}

void ATurretPawn::TurnTurret(const FRotator& InValue)
{
	const FRotator Rotation = FMath::RInterpTo(TurretMesh->GetComponentRotation(), InValue,
		GetWorld()->GetDeltaSeconds(), TurretRotationSpeed);
	bool bIsRotationsEqual = false;

	if(FMath::IsNearlyEqual(InValue.Yaw, TurretMesh->GetComponentRotation().Yaw, 0.5))
	{
		bIsRotationsEqual = true;
	}
	TurretMesh->SetWorldRotation(Rotation);

	if(!IsTurretTurningSoundSpawned() && !bIsRotationsEqual)
	{
		SpawnTurretTurningSound(GetActorLocation(), GetActorRotation());
	}
	
	if(bIsRotationsEqual)
	{
		DestroyTurretTurningSound();
	}
}

void ATurretPawn::Fire()
{
	GetWorld()->SpawnActor<AProjectile>(ProjectileToSpawn, ProjectileSpawnPoint->GetComponentTransform());

	SpawnShootParticle(ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	SpawnShootSound(ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
}

void ATurretPawn::CheckHealth(float CurrentHealth)
{
	if(FMath::IsNearlyZero(CurrentHealth))
	{
		Destroy();

		SpawnDeathParticle(GetActorLocation(), GetActorRotation());
		SpawnOnDeathSound(GetActorLocation(), GetActorRotation());
		DestroyAmbientSound();
	}
}

void ATurretPawn::SpawnShootParticle(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootParticle, Location, Rotation);
}

void ATurretPawn::SpawnHitParticle(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Location, Rotation);
}

void ATurretPawn::SpawnDeathParticle(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, Location, Rotation);
}

void ATurretPawn::SpawnHitSound(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Location, Rotation);
}

void ATurretPawn::SpawnShootSound(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound, Location, Rotation);
}

void ATurretPawn::SpawnOnDeathSound(const FVector& Location, const FRotator& Rotation) const
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), OnDeathSound, Location, Rotation);
}

void ATurretPawn::SpawnTurretTurningSound(const FVector& Location, const FRotator& Rotation)
{
	TurretTurningSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), TurretTurningSound, Location, Rotation);
}

void ATurretPawn::SpawnAmbientSound(const FVector& Location, const FRotator& Rotation)
{
	AmbientSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AmbientSound, Location, Rotation);
}

void ATurretPawn::DestroyTurretTurningSound()
{
	if(TurretTurningSoundComponent)
	{
		TurretTurningSoundComponent->Deactivate();
		TurretTurningSoundComponent = nullptr;
	}
}

void ATurretPawn::DestroyAmbientSound()
{
	if(AmbientSoundComponent)
	{
		AmbientSoundComponent->Deactivate();
		AmbientSoundComponent = nullptr;
	}
}

bool ATurretPawn::IsTurretTurningSoundSpawned() const
{
	return TurretTurningSoundComponent != nullptr;
}