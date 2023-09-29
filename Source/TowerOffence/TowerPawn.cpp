// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerPawn.h"

#include "Engine/OverlapInfo.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ATowerPawn::ATowerPawn()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision sphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetVisibility(true);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATowerPawn::OnBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ATowerPawn::OnEndOverlap);
}

void ATowerPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SetCollisionSphereRadius();
}

void ATowerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const TObjectPtr<AActor> Player = GetClosestTarget();
	if(Player)
	{
		FRotator Rotation(0.0f);
		GetRotation(Player, &Rotation);
		TurnTurret(Rotation);
		if(TurretMesh->GetPhysicsAngularVelocityInDegrees().Equals(FVector(0.0f)) && TimeAfterLastShot <= 0.0f)
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

void ATowerPawn::GetRotation(const TObjectPtr<AActor> Player, FRotator* Rotation) const
{
	*Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
	Rotation->Roll = 0.0f;
	Rotation->Pitch = 0.0f;
	Rotation->Yaw -= 90.0f;
}

TObjectPtr<AActor> ATowerPawn::GetClosestTarget() const
{
	if(!PlayerRef.IsEmpty())
	{
		TObjectPtr<AActor> ClosestTarget = PlayerRef[0];
		float DistanceToClosestTarget = GetDistanceToTarget(ClosestTarget);
		for(int i = 1; i < PlayerRef.Num(); ++i)
		{
			const float Distance = GetDistanceToTarget(PlayerRef[i]);
			if(Distance < DistanceToClosestTarget)
			{
				ClosestTarget = PlayerRef[i];
				DistanceToClosestTarget = Distance;
			}
		}
		return ClosestTarget;
	}
	return nullptr;
}

float ATowerPawn::GetDistanceToTarget(const AActor* OtherActor) const
{
	return OtherActor ? GetSizeOfVector(GetActorLocation() - OtherActor->GetActorLocation()) : 0.f;
}

float ATowerPawn::GetSizeOfVector(const FVector& InVector) const
{
	return pow(InVector.X, 2) + pow(InVector.Y, 2) + pow(InVector.Z, 2);
}

void ATowerPawn::SetCollisionSphereRadius() const
{
	CollisionSphere->SetSphereRadius(CollisionSphereRadius);
}

void ATowerPawn::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(this, 1);
	UKismetSystemLibrary::SphereOverlapActors(CollisionSphere, CollisionSphere->GetComponentLocation(),
		CollisionSphereRadius, TraceObjectTypes, nullptr, IgnoreActors, PlayerRef);
}

void ATowerPawn::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	PlayerRef.RemoveSingle(OtherActor);
}

void ATowerPawn::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("ATowerPawn::Fire() was called"));
}

