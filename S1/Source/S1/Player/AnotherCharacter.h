// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Struct.pb.h"
#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "AnotherCharacter.generated.h"

UCLASS()
class S1_API AAnotherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnotherCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveActor(const Protocol::PlayerInfo& PlayerInfo);

private:
	void MoveWALK(const Protocol::PlayerInfo& PlayerInfo);
	void MoveIDLE(const Protocol::PlayerInfo& PlayerInfo);

private:
	Protocol::MoveType MoveType = Protocol::MoveType::PLAYER_IDLE;
};
