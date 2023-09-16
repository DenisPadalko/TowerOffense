// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

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
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ATankPawn::Move);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Completed, this, &ATankPawn::FinishMoving);
		EnhancedInputComponent->BindAction(TurnRightAction, ETriggerEvent::Triggered, this, &ATankPawn::Turn);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATankPawn::Fire);
	}
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
	FHitResult HitResult;
	if(PlayerController)
	{
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentTransform().GetLocation(), HitResult.ImpactPoint);
		Rotation.Roll = 0.0f;
		Rotation.Pitch = 0.0f;
		Rotation.Yaw -= 90.0f;
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 500, 16, FColor::Red);
		TurnTurret(Rotation, DeltaSeconds, TurretRotationSpeed);
	}
}

void ATankPawn::Move(const FInputActionValue& InValue)
{
	MovementTime += GetWorld()->GetDeltaSeconds();
	FVector MovementVector(0.0f, 0.0f, 0.0f);
	const float Alpha = FMath::Clamp(MovementTime / TimeToMove, 0, 1);
	MovementVector.Y = FMath::Lerp(0.0f, MovementSpeed * InValue.Get<FVector>().Y, Alpha);
	AddActorLocalOffset(MovementVector);
}

void ATankPawn::FinishMoving()
{
	MovementTime = 0.0f;
}

void ATankPawn::Turn(const FInputActionValue& InValue)
{
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * InValue.Get<FVector>().X, 0.0f));
}

void ATankPawn::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire action was called"));
}