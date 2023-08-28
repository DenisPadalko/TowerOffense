// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"

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
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

TArray<FName> ATurretPawn::GetNameOptions() const
{
	return TurretMesh->GetMaterialSlotNames();
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	
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
	const TObjectPtr<UMaterialInterface> Material = TurretMesh->GetMaterial(MaterialIndex);
	if(!DynamicMaterialInstance)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
	}
	DynamicMaterialInstance->SetVectorParameterValue(ParameterName, ColorOfTeam);
	TurretMesh->SetMaterial(MaterialIndex, DynamicMaterialInstance);
	BaseMesh->SetMaterial(BaseMesh->GetMaterialIndex(MaterialSlotName), DynamicMaterialInstance);
}

