// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h" // CoreMinimal.h
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* Trigger;
	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 배치한 아이템 상자에 클래스정보를 저장할 속성을 추가 -> 이 값을 기반으로 플레이어가 상자와 겹치면 아이템을 생성하도록 구현
	UPROPERTY(VisibleAnywhere, Category = Box)
	TSubclassOf<class AABWeapon> WeaponItemClass; 
	// UClass 포인터를 사용해도 되지만 그러면 현재 프로젝트에 사용하는 모든 오브젝트의 선언이 보임
	// TSubclassOf 는 특정 클래스와 상속받은 클래스들로 목록을 한정하는 키워드임
};
