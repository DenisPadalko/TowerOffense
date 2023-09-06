// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Camera->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetupAttachment(SpringArmComponent);
	
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->TargetArmLength = 500.0f;
}

