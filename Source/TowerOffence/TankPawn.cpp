// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "CustomGameModeBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArmComponent);

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATankPawn::CallFire);
	}
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	TObjectPtr<ACustomGameModeBase> GameMode = Cast<ACustomGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->OnPawnCreated(this);
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentTransform().GetLocation(), HitResult.ImpactPoint);
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
}

float ATankPawn::GetCurrentSpeed() const
{
	const float Alpha = FMath::Clamp(MovementTime / AccelerationDuration, 0, 1);
	return FMath::Lerp(0.0f, MovementSpeed, Alpha);
}

void ATankPawn::FinishMoving()
{
	MovementTime = 0.0f;
}

void ATankPawn::Turn(const FInputActionValue& InValue)
{
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * InValue.Get<float>(), 0.0f));
}

void ATankPawn::CallFire()
{
	if(TimeAfterLastShot <= 0.0f)
	{
		Fire();
		TimeAfterLastShot = TimeBetweenShots;
	}
}

void ATankPawn::CheckHealth(float CurrentHealth)
{
	if(HealthComponent->IsZero())
	{
		TObjectPtr<ACustomGameModeBase> GameMode = Cast<ACustomGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->OnPawnKilled(this);
	}
	ATurretPawn::CheckHealth(CurrentHealth);
}
