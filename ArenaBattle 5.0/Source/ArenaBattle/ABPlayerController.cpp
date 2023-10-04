// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	ABLOG_S(Warning);
	Super::OnPossess(aPawn);
}


// 테스트 플레이 시작시 매번 언리얼 엔진의 뷰포트를 클릭하는 과정을 없애기위해서 작성
// 플레이어 컨트롤러에게 UI를 배제하고 게임에게만 입력을 전달하도록 명령을 내리면됨
void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

