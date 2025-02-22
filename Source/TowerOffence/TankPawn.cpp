// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "CustomGameModeBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArmComponent);

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LeftDustSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Left dust spawn point"));
	LeftDustSpawnPoint->SetupAttachment(BaseMesh);

	RightDustSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Right dust spawn point"));
	RightDustSpawnPoint->SetupAttachment(BaseMesh);
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		if(TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
		
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
	
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ATankPawn::InputMove);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Completed, this, &ATankPawn::FinishMoving);
		EnhancedInputComponent->BindAction(TurnRightAction, ETriggerEvent::Triggered, this, &ATankPawn::Turn);
		EnhancedInputComponent->BindAction(TurnRightAction, ETriggerEvent::Completed, this, &ATankPawn::FinishTurn);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATankPawn::CallFire);
	}
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	TObjectPtr<ACustomGameModeBase> GameMode = Cast<ACustomGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->OnPawnCreated(this);

	GameMode->OnBeginPlay();
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentTransform().GetLocation(),
			HitResult.ImpactPoint);
		Rotation.Roll = 0.0f;
		Rotation.Pitch = 0.0f;
		Rotation.Yaw -= 90.0f;
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 500, 16, FColor::Red);
		TurnTurret(Rotation);
	}
	
	TimeAfterLastShot -= DeltaSeconds;
}

void ATankPawn::InputMove(const FInputActionValue& InValue)
{
	Move(InValue.Get<float>());
}

void ATankPawn::Move(const float Direction)
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	MovementTime += DeltaTime;

	const float Speed = GetCurrentSpeed();
	const FVector Velocity = GetActorLocation().ForwardVector * Direction * Speed;

	const FVector DeltaMove = Velocity * DeltaTime;
	AddActorLocalOffset(DeltaMove);

	SpawnDustFromTank({LeftDustSpawnPoint, RightDustSpawnPoint});

	if(!IsMovementSoundSpawned())
	{
		SpawnMovementSound(GetActorLocation(), GetActorRotation());
	}
}

void ATankPawn::SpawnDustFromTank(const TArray<TObjectPtr<USceneComponent>> AttachToComponent)
{
	if(LeftDustFromTankComponent || RightDustFromTankComponent)
	{
		return;
	}

	if(AttachToComponent[0])
	{
		SpawnLeftDustFromTankComponent(AttachToComponent[0]);
	}

	if(AttachToComponent[1])
	{
		SpawnRightDustFromTankComponent(AttachToComponent[1]);
	}
}

void ATankPawn::SpawnLeftDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent)
{
	LeftDustFromTankComponent = UGameplayStatics::SpawnEmitterAttached(DustFromTank, AttachToComponent, FName("Dust point"),
		FVector(ForceInit), FRotator::ZeroRotator, FVector(1), EAttachLocation::KeepRelativeOffset,
		false, EPSCPoolMethod::None, false);
	LeftDustFromTankComponent->Activate(true);
}

void ATankPawn::SpawnRightDustFromTankComponent(const TObjectPtr<USceneComponent> AttachToComponent)
{
	RightDustFromTankComponent = UGameplayStatics::SpawnEmitterAttached(DustFromTank, AttachToComponent, FName("Dust point"),
		FVector(ForceInit), FRotator::ZeroRotator, FVector(1), EAttachLocation::KeepRelativeOffset,
		false, EPSCPoolMethod::None, false);
	RightDustFromTankComponent->Activate(true);
}

void ATankPawn::DestroyDustFromTank()
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

void ATankPawn::DestroyMovementSound()
{
	if(MovementSoundComponent)
	{
		MovementSoundComponent->Deactivate();
		MovementSoundComponent = nullptr;
	}
}

bool ATankPawn::IsMovementSoundSpawned() const
{
	return MovementSoundComponent != nullptr;
}

void ATankPawn::SpawnMovementSound(const FVector& Location, const FRotator& Rotation)
{
	MovementSoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), MovementSound, Location, Rotation);
}

float ATankPawn::GetCurrentSpeed() const
{
	const float Alpha = FMath::Clamp(MovementTime / AccelerationDuration, 0, 1);
	return FMath::Lerp(0.0f, MovementSpeed, Alpha);
}

void ATankPawn::FinishMoving()
{
	MovementTime = 0.0f;

	DestroyDustFromTank();

	if(IsMovementSoundSpawned())
	{
		DestroyMovementSound();
	}
}

void ATankPawn::Turn(const FInputActionValue& InValue)
{
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * InValue.Get<float>(), 0.0f));
	
	SpawnDustFromTank({LeftDustSpawnPoint, RightDustSpawnPoint});

	if(!IsMovementSoundSpawned())
	{
		SpawnMovementSound(GetActorLocation(), GetActorRotation());
	}
}

void ATankPawn::FinishTurn()
{
	DestroyDustFromTank();
	
	if(IsMovementSoundSpawned())
	{
		DestroyMovementSound();
	}
}


void ATankPawn::CallFire()
{
	if(TimeAfterLastShot <= 0.0f)
	{
		Fire();
		Projectile->OnTargetHit.BindUObject(this, &ATankPawn::IsHitActorDead);

		ShakeCamera();

		TimeAfterLastShot = TimeBetweenShots;
	}
}

void ATankPawn::ShakeCamera()
{
	if(CameraShake)
	{
		const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
		PlayerController->PlayerCameraManager->PlayWorldCameraShake(GetWorld(), CameraShake, Camera->GetComponentLocation(),
			CameraShakeInnerRadius, CameraShakeOuterRadius, CameraShakeFalloff);
	}
}

void ATankPawn::CheckHealth(float CurrentHealth)
{
	if(FMath::IsNearlyZero(CurrentHealth))
	{
		TObjectPtr<ACustomGameModeBase> GameMode = Cast<ACustomGameModeBase>(UGameplayStatics::GetGameMode(this));
		GameMode->OnPawnKilled(this);

		ShakeCamera();
	}
	
	Super::CheckHealth(CurrentHealth);
}

void ATankPawn::IsHitActorDead(AActor* HitActor)
{
	const TObjectPtr<UHealthComponent> HealthComponentOfActor = HitActor->FindComponentByClass<UHealthComponent>();
	if(HealthComponentOfActor && HealthComponentOfActor->IsZero())
	{
		ShakeCamera();
	}
}
