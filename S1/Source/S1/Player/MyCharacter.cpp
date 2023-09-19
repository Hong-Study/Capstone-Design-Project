// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "S1.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshResource(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (MeshResource.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshResource.Object);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	// Pawn
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// SpringArm
	SpringArm->SetRelativeRotation(FRotator(-30, 0, 0));
	SpringArm->TargetArmLength = 700.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;

	// Input
	ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (MappingContext.Succeeded())
		InputMappingContext = MappingContext.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> MoveContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (MoveContext.Succeeded())
		MoveAction = MoveContext.Object;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	if (PlayerController == nullptr)
		return;

	auto* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(InputMappingContext, 0);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (MoveType == Protocol::MoveType::PLAYER_WALK)
	{
		if (US1GameInstance* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
		{
			// TODO
			GameInstance->SendMovePacket(GetActorLocation(), GetActorRotation().Yaw, MoveType);
		}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::IA_Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyCharacter::IA_Move_Stop);
	}
}

void AMyCharacter::IA_Move(const FInputActionValue& Value)
{
	if (MoveType != Protocol::MoveType::PLAYER_WALK)
	{
		if (US1GameInstance* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
		{
			// TODO
			GameInstance->SendMovePacket(GetActorLocation(), GetActorRotation().Yaw, MoveType);
		}
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (MovementVector.X != 0)
	{
		FRotator Rotator = GetControlRotation();
		FVector Direction = UKismetMathLibrary::GetForwardVector(FRotator(0, Rotator.Yaw, 0));
		AddMovementInput(Direction, MovementVector.X);
	}

	if (MovementVector.Y != 0)
	{
		FRotator Rotator = GetControlRotation();
		FVector Direction = UKismetMathLibrary::GetRightVector(FRotator(0, Rotator.Yaw, 0));
		AddMovementInput(Direction, MovementVector.Y);
	}

	MoveType = Protocol::MoveType::PLAYER_WALK;
}

void AMyCharacter::IA_Move_Stop(const FInputActionValue& Value)
{
	if (MoveType != Protocol::MoveType::PLAYER_IDLE)
	{
		if (US1GameInstance* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
		{
			// TODO
			GameInstance->SendMovePacket(GetActorLocation(), GetActorRotation().Yaw, MoveType);
		}
	}

	MoveType = Protocol::MoveType::PLAYER_IDLE;
}

void AMyCharacter::IA_Turn(const FInputActionValue& Value)
{

}

