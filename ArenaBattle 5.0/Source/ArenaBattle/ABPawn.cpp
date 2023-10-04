// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPawn.h"

// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.0f);// ĳ������ ���ݳ���
	Capsule->SetCapsuleRadius(34.0f);// ĳ������ ���ѷ�
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//ĳ���� ���� ����Ʈ������ 3���� ��ǥ�� �𸮾��� 3���� ��ǥ�� �ٸ��⶧���� z������ -90�� ȸ�����������
	//�𸮾󿡼� ������ ������ġ�� ���߾������� ĳ���� ������ �߹ٴ��� �������� ��⶧���� z������ ���ݳ��̸�ŭ ��������
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	//�������Ʈ�� ���� �ִϸ��̼��� ���� �����Ű�� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		WARRIOR_ANIM(TEXT("/Game/Book/Animation/WarriorAnimBlueprint.WarriorAnimBlueprint_C")); // Ŭ���� ������ ���������� ��γ��� C�� �پ������
	if (WARRIOR_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}
}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();
	//Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);// ĳ���Ϳ� �޸��� �ִϸ��̼��� ��������
	//UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/Book/Animation/WarriorRun.WarriorRun"));
	//if (AnimAsset != nullptr)
	//{
	//	Mesh->PlayAnimation(AnimAsset, true);
	//}
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//InputCompunent�� ������ �Է½�ȣ�� �������ִµ� �װ� ���� ����Լ��� �����������.
	//�װ� �����ϴ� ���� ���� SetupInputComponent��
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);
}

void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPawn::PossessedBy(AController* NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}

void AABPawn::UpDown(float NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	AddMovementInput(GetActorForwardVector(), NewAxisValue); // �̰� �־�� ĳ���Ͱ� ��¥�� ������
	//GetActorForwardVector()�� ���� ��ǥ�� �������� ������ ���� ������ ���� �����͸� ����������
	//���� ���ϸ� �����Ʈ ������Ʈ�� �ڵ��� ����, AddMovementInput����� �������� �������̽� 1�� ���� -1�� ���� 0�� �������� ������
	//GetActorForwardVector()�� �ڵ��� �ڵ���
}

void AABPawn::LeftRight(float NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}

