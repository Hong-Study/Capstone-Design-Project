// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h" // CoreMinimal.h 였음
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// 델리게이트를 사용하면 의존성 없는 설계를 할수있는 장점이 있다.
// 반환 값과 인자 값이 없는 함수 유형으로 선언하되, 여러 개의 함수가 등록되도록 멀티캐스트로 선언했음
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate); 
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; // 애님 인스턴스의 tick함수에서 폰의 속도정보를 가져오기위해 선언

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION() // 이걸로 적어야 언리얼 런타임이 감지할수있음
	void AnimNotify_AttackHitCheck(); // 애님인스턴스에서 애니메이션 노티파이를 감지하도록 하기위해서 선언한 코드

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta = (AllowPrivateAccess = true)) // 폰의 현재속력을 받아서 저장할 변수
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	
};
