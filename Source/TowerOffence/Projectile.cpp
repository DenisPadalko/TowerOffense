// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "TowerPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(StaticMeshComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->bAutoActivate = true;

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::Destroy);
}

void AProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ProjectileMovementComponent->InitialSpeed = SpeedOfMovement;
	ProjectileMovementComponent->MaxSpeed = SpeedOfMovement;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::Destroy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector Impulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, StaticClass());

	OnTargetHit.Execute(OtherActor);
	
	AActor::Destroy();
}
