// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	AABGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override; //  플레이어 입장과정 로그를 보기위한 코드
	/*플레이어가 로그인을 완료하면 게임모드의 PostLogin 이벤트함수가 호출됨
	이 함수 내부에서는 폰을 생성하고 플레이어 컨트롤러가 폰에 빙의하는 작업이 이루어짐*/
};
