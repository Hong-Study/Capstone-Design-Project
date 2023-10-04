// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override; // �׽�Ʈ �÷��� ���۽� �Ź� �𸮾� ������ ����Ʈ�� Ŭ���ϴ� ������ ���ֱ����ؼ� �ۼ�

public:
	virtual void PostInitializeComponents() override; // �α׸� �������� ������ �ڵ�
	virtual void OnPossess(APawn* aPawn) override; // �α׸� �������� ������ �ڵ�
	
};
