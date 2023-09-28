// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerPawn.h"

#include "Engine/OverlapInfo.h"
#include "Kismet/KismetMathLibrary.h"

ATowerPawn::ATowerPawn()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision sphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetVisibility(true);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATowerPawn::OnBeginOverlap);
}

void ATowerPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SetCollisionSphereRadius();
}

void ATowerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(PlayerRef)
	{
		float Distance = GetDistanceTo(PlayerRef);
		UE_LOG(LogTemp, Warning, TEXT("Dist: %f"), Distance);
		if(Distance && Distance <= CollisionSphereRadius)
		{
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerRef->GetActorLocation());
			Rotation.Roll = 0.0f;
			Rotation.Pitch = 0.0f;
			Rotation.Yaw -= 90.0f;
			TurnTurret(Rotation);

			if(TimeAfterLastShot <= 0.0f)
			{
				TimeAfterLastShot = TimeBetweenShots;
				Fire();
			}
			else
			{
				TimeAfterLastShot -= GetWorld()->GetDeltaSeconds();
			}
		}
	}
}

void ATowerPawn::SetCollisionSphereRadius() const
{
	CollisionSphere->SetSphereRadius(CollisionSphereRadius);
}

void ATowerPawn::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerRef = OtherActor;
}

void ATowerPawn::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("ATowerPawn::Fire() was called"));
}

