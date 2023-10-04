// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABCharacter.h" // ABPawn.h에서 변경
#include "ABPlayerController.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass(); // AABPawn에서 변경
	PlayerControllerClass = AABPlayerController::StaticClass();
	
	/*static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
		if (BP_PAWN_C.Succeeded())
		{
			DefaultPawnClass = BP_PAWN_C.Class;
		}
		블루프린트로 만들어둔 폰을 기본 폰으로 사용하려고 할때
		*/


}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}