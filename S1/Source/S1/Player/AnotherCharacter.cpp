// Fill out your copyright notice in the Description page of Project Settings.


#include "AnotherCharacter.h"

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

// Called to bind functionality to input
void AAnotherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

