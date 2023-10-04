// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h" // CoreMinimal.h ����
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

// ��������Ʈ�� ����ϸ� ������ ���� ���踦 �Ҽ��ִ� ������ �ִ�.
// ��ȯ ���� ���� ���� ���� �Լ� �������� �����ϵ�, ���� ���� �Լ��� ��ϵǵ��� ��Ƽĳ��Ʈ�� ��������
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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; // �ִ� �ν��Ͻ��� tick�Լ����� ���� �ӵ������� ������������ ����

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION() // �̰ɷ� ����� �𸮾� ��Ÿ���� �����Ҽ�����
	void AnimNotify_AttackHitCheck(); // �ִ��ν��Ͻ����� �ִϸ��̼� ��Ƽ���̸� �����ϵ��� �ϱ����ؼ� ������ �ڵ�

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta = (AllowPrivateAccess = true)) // ���� ����ӷ��� �޾Ƽ� ������ ����
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	
};
