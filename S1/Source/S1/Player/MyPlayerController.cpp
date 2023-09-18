// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AMyPlayerController::AMyPlayerController()
{
	// Input
	ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (MappingContext.Succeeded())
		InputMappingContext = MappingContext.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> MoveContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (MoveContext.Succeeded())
		MoveAction = MoveContext.Object;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::IA_Move);
	}
}

void AMyPlayerController::IA_Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (MovementVector.X != 0)
	{
		FRotator Rotator = GetControlRotation();
		FVector Direction = UKismetMathLibrary::GetForwardVector(FRotator(0, Rotator.Yaw, 0));
		GetPawn()->AddMovementInput(Direction, MovementVector.X);
	}

	if (MovementVector.Y != 0)
	{
		FRotator Rotator = GetControlRotation();
		FVector Direction = UKismetMathLibrary::GetRightVector(FRotator(0, Rotator.Yaw, 0));
		GetPawn()->AddMovementInput(Direction, MovementVector.Y);
	}
}

void AMyPlayerController::IA_Turn(const FInputActionValue& Value)
{

}
