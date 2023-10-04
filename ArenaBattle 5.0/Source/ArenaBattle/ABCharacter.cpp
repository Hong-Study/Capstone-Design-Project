// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h" // 공격 애니메이션 재생을 위해서 추가함
#include "ABWeapon.h"
#include "DrawDebugHelpers.h" // 디버그 드로잉을 사용하기위해서 추가함

// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// /Game/Book/Animation/WarriorIdleBlueprint.WarriorIdleBlueprint_C
	// /Game/Book/Animation/WarriorAnimBlueprint.WarriorAnimBlueprint_C

	//블루프린트로 만든 애니메이션을 폰에 적용시키는 과정
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		WARRIOR_ANIM(TEXT("/Game/Book/Animation/WarriorAnimBlueprint.WarriorAnimBlueprint_C")); // 클래스 정보를 가져오려면 경로끝에 C를 붙어줘야함
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	//SetControlMode(0);
	SetControlMode(EControlMode::GTA);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;

	MaxCombo = 4;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter")); // 캡슐 컴포넌트가 우리가 만든 프리셋을 사용하도록 기본값을 지정함

	AttackRadius = 50.0f;
	AttackRange = 200.0f;

	//FName WeaponSocket(TEXT("hand_rSocket")); // 스켈레탈 메시에 무기 달아주기
	//if (GetMesh()->DoesSocketExist(WeaponSocket))
	//{
	//	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	//	if (SK_WEAPON.Succeeded())
	//	{
	//		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	//	}

	//	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	//}
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator); // 월드에 새롭게 액터를 생성하는 명령은 SpawnActor
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	}
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	//switch (CurrentControlMode)
	//{
	//case EControlMode::DIABLO:
	//	SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed)); // 책 p203 코드랑 조금다름
	//	break;
	//}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed)); // 책 p203 코드랑 조금다름
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}

}

bool AABCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this); // 무기액터의 소유자를 캐릭터로 변경
		CurrentWeapon = NewWeapon;
	}
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
}

void AABCharacter::UpDown(float NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	//AddMovementInput(GetActorForwardVector(), NewAxisValue); // 이게 있어야 캐릭터가 진짜로 움직임
	//GetActorForwardVector()는 월드 좌표계 기준으로 액터의 전진 방향의 벡터 데이터를 가지고있음
	//쉽게 말하면 무브먼트 컴포넌트는 자동차 엔진, AddMovementInput명령은 운전석의 인터페이스 1은 전진 -1은 후진 0은 엑셀에서 발을땜
	//GetActorForwardVector()는 자동차 핸들임

	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	//AddMovementInput(GetActorRightVector(), NewAxisValue);

	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

void AABCharacter::Turn(float NewAxisValue) // Turn은 캐릭터의 z축 회전
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	//AddControllerYawInput(NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}

void AABCharacter::LookUp(float NewAxisValue) // LookUp은 캐릭터의 y축 회전
{
	//ABLOG(Warning, TEXT("%f"), NewAxisValue);
	//AddControllerPitchInput(NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

void AABCharacter::SetControlMode(EControlMode NewControlMode) //시점설정
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true; //캐릭터가 움직이는 방향으로 자동으로 캐릭터를 회전시켜주는 기능
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f));
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true; // 이게 true여야 캐릭터가 부드럽게 회전함 false면 45도 각도로 끊어져서 회전함
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}

	//if (ControlMode == 0)
	//{
	//	SpringArm->TargetArmLength = 450.0f;
	//	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	//	SpringArm->bUsePawnControlRotation = true;
	//	SpringArm->bInheritPitch = true;
	//	SpringArm->bInheritRoll = true;
	//	SpringArm->bInheritYaw = true;
	//	SpringArm->bDoCollisionTest = true;
	//	bUseControllerRotationYaw = false;
	//	GetCharacterMovement()->bOrientRotationToMovement = true; //캐릭터가 움직이는 방향으로 자동으로 캐릭터를 회전시켜주는 기능
	//	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	//}
}

void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation()); //  책 p204 코드랑 조금다름
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::Attack()
{
	//ABLOG_S(Warning);
	if (IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);

	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {ABLOG(Warning, TEXT("OnNextAttackCheck"));
	CanNextCombo = false;
	if (IsComboInputOn)
	{
		AttackStartComboState();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
	}

	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);
	
});
}

void AABCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this); // 충돌감지에서 플레이어 본인이 감지되면 안되므로 this를 제외시켜줌
	bool bResult = GetWorld()->SweepSingleByChannel( // 물리는 월드의 기능이므로 GetWolrd 함수로 명령을 내려야함
		HitResult,
		GetActorLocation(), // 감지 시작위치
		GetActorLocation() + GetActorForwardVector() * 200.0f, // 감지 종료 위치
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f), // 탐색에 사용할 기본 도형의 정보  /우리는 반지름 50cm의 원을만들었음
		Params);

	#if ENABLE_DRAW_DEBUG

		FVector TraceVec = GetActorForwardVector() * AttackRange;
		FVector Center = GetActorLocation() + TraceVec * 0.5f;
		float HalfHeight = AttackRange * 0.5f + AttackRadius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red; // 공격판정이 발생하면 녹색으로 표시, 아니면 붉은색으로 표시
		float DebufLifeTime = 5.0f;

		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			AttackRadius,
			CapsuleRot,
			DrawColor,
			false,
			DebufLifeTime);
	#endif

		if (bResult)
		{
		// UE4에서는 WeakPointer를 기반 하였는데 UE5에서 FActorInstanceHandle로 구현이 변경 되어서 사용 코드도 변경됨
			if (HitResult.GetActor()) // UE4에서는Actor.Isvalid() 였는데 UE5에서는 GetActor()로 변경됨
			{
				ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName()); // UE4에서는Actor->GetName() 였는데 UE5에서는 GetActor()->GetName()로 변경됨

				FDamageEvent DamageEvent; // 데미지를 전달하는 함수 but  피해를 입은 액터의 관련 로직도 만들어줘야 데미지처리가 구현됨
				HitResult.GetActor()->TakeDamage(50.0f, DamageEvent, GetController(), this); // 데미지의 세기/ 데미지 종류 / 공격명령을 내린 가해자/ 데미지 전달을 위해 사용된 도구
			}
		}

}

float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// TakeDamage 함수의 부모인 AActor 에 데미지관련 기본 로직이 구현되어있어서 super로 부모의 로직을 먼저 실행시켜줘야함
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	ABLOG(Warning, TEXT("took Damage : %f"), FinalDamage); // *GetName 사용이 안되는데 버전이 올라가면서 사용법이 바뀐듯? 찾아봐야함

	if (FinalDamage > 0.0f)
	{
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false); // 이게 있어야 죽은 시체에 충돌이벤트가 발생하지않음
	}

	return FinalDamage;
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1)); // test value, min, max
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}