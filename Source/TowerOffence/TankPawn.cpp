// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	}
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ATankPawn::Move);
		EnhancedInputComponent->BindAction(TurnRightAction, ETriggerEvent::Triggered, this, &ATankPawn::Turn);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATankPawn::Fire);
	}
}

void ATankPawn::Move(const FInputActionValue& InValue)
{
	FVector MovementVector(0.0f, 0.0f, 0.0f);
	const float Alpha = FMath::Clamp(RunningTime / TimeToMove, 0, 1);
	RunningTime += GetWorld()->GetDeltaSeconds();
	MovementVector.Y = FMath::Lerp(0.0f, MovementSpeed * InValue.Get<FVector>().Y, Alpha);
	AddActorLocalOffset(MovementVector);
}

void ATankPawn::Turn(const FInputActionValue& InValue)
{
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * InValue.Get<FVector>().X, 0.0f));
}

void ATankPawn::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire action was called"));
}

void ATankPawn::Look(const FInputActionValue& InValue)
{
	const FVector2D LookAxisVector = InValue.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}
