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
	virtual void BeginPlay() override; // 테스트 플레이 시작시 매번 언리얼 엔진의 뷰포트를 클릭하는 과정을 없애기위해서 작성

public:
	virtual void PostInitializeComponents() override; // 로그를 보기위해 선언한 코드
	virtual void OnPossess(APawn* aPawn) override; // 로그를 보기위해 선언한 코드
	
};
