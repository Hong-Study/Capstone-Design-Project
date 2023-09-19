// Fill out your copyright notice in the Description page of Project Settings.


#include "AnotherCharacter.h"
#include "S1.h"

// Sets default values
AAnotherCharacter::AAnotherCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshResource(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (MeshResource.Succeeded())
		GetMesh()->SetSkeletalMesh(MeshResource.Object);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
}

// Called when the game starts or when spawned
void AAnotherCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAnotherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// fCalled to bind functionality to input
void AAnotherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAnotherCharacter::MoveActor(const Protocol::PlayerInfo& PlayerInfo)
{
	switch (PlayerInfo.movetype())
	{
	case Protocol::MoveType::PLAYER_IDLE:
		MoveIDLE(PlayerInfo);
		break;
	case Protocol::MoveType::PLAYER_WALK:
		MoveWALK(PlayerInfo);
		break;
	
	default:
		break;
	}
}

void AAnotherCharacter::MoveWALK(const Protocol::PlayerInfo& PlayerInfo)
{
	FVector MovePos(PlayerInfo.x(), PlayerInfo.y(), PlayerInfo.z());
	FRotator Rotator(0.0f, PlayerInfo.yaw(), 0.f);

	FVector NowLocation = GetActorLocation();
	if (NowLocation == MovePos)
		return;

	FVector MoveDirection = (MovePos - NowLocation).GetSafeNormal();
	AddMovementInput(MoveDirection);

	SetActorLocation(MovePos);

	SetActorRotation(Rotator);

	MoveType = PlayerInfo.movetype();

	//float MoveDistance = FVector::Distance(MoveDirection, NowLocation);

	//// 이동 거리에 따른 Scale 계산 (예: 거리가 100.0f일 때 Scale 1.0f)
	//float Scale = 1.0f;
	//if (MoveDistance > 0.0f)
	//{
	//	Scale = MoveDistance / 100.0f; // 이동 거리가 100.0f일 때 Scale 1.0f
	//}

	//AddMovementInput(MoveDirection, Scale);
}

void AAnotherCharacter::MoveIDLE(const Protocol::PlayerInfo& PlayerInfo)
{
	if (MoveType != PlayerInfo.movetype())
	{
		MoveWALK(PlayerInfo);
	}

	//MoveType = PlayerInfo.movetype();
}

