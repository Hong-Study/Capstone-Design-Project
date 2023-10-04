// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h" // ���� �ִϸ��̼� ����� ���ؼ� �߰���
#include "ABWeapon.h"
#include "DrawDebugHelpers.h" // ����� ������� ����ϱ����ؼ� �߰���

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

	//�������Ʈ�� ���� �ִϸ��̼��� ���� �����Ű�� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		WARRIOR_ANIM(TEXT("/Game/Book/Animation/WarriorAnimBlueprint.WarriorAnimBlueprint_C")); // Ŭ���� ������ ���������� ��γ��� C�� �پ������
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

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter")); // ĸ�� ������Ʈ�� �츮�� ���� �������� ����ϵ��� �⺻���� ������

	AttackRadius = 50.0f;
	AttackRange = 200.0f;

	//FName WeaponSocket(TEXT("hand_rSocket")); // ���̷�Ż �޽ÿ� ���� �޾��ֱ�
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
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator); // ���忡 ���Ӱ� ���͸� �����ϴ� ����� SpawnActor
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
	//	SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed)); // å p203 �ڵ�� ���ݴٸ�
	//	break;
	//}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed)); // å p203 �ڵ�� ���ݴٸ�
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
		NewWeapon->SetOwner(this); // ��������� �����ڸ� ĳ���ͷ� ����
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
	//AddMovementInput(GetActorForwardVector(), NewAxisValue); // �̰� �־�� ĳ���Ͱ� ��¥�� ������
	//GetActorForwardVector()�� ���� ��ǥ�� �������� ������ ���� ������ ���� �����͸� ����������
	//���� ���ϸ� �����Ʈ ������Ʈ�� �ڵ��� ����, AddMovementInput����� �������� �������̽� 1�� ���� -1�� ���� 0�� �������� ������
	//GetActorForwardVector()�� �ڵ��� �ڵ���

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

void AABCharacter::Turn(float NewAxisValue) // Turn�� ĳ������ z�� ȸ��
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

void AABCharacter::LookUp(float NewAxisValue) // LookUp�� ĳ������ y�� ȸ��
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

void AABCharacter::SetControlMode(EControlMode NewControlMode) //��������
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
		GetCharacterMovement()->bOrientRotationToMovement = true; //ĳ���Ͱ� �����̴� �������� �ڵ����� ĳ���͸� ȸ�������ִ� ���
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
		GetCharacterMovement()->bUseControllerDesiredRotation = true; // �̰� true���� ĳ���Ͱ� �ε巴�� ȸ���� false�� 45�� ������ �������� ȸ����
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
	//	GetCharacterMovement()->bOrientRotationToMovement = true; //ĳ���Ͱ� �����̴� �������� �ڵ����� ĳ���͸� ȸ�������ִ� ���
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
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation()); //  å p204 �ڵ�� ���ݴٸ�
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
	FCollisionQueryParams Params(NAME_None, false, this); // �浹�������� �÷��̾� ������ �����Ǹ� �ȵǹǷ� this�� ���ܽ�����
	bool bResult = GetWorld()->SweepSingleByChannel( // ������ ������ ����̹Ƿ� GetWolrd �Լ��� ����� ��������
		HitResult,
		GetActorLocation(), // ���� ������ġ
		GetActorLocation() + GetActorForwardVector() * 200.0f, // ���� ���� ��ġ
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f), // Ž���� ����� �⺻ ������ ����  /�츮�� ������ 50cm�� �����������
		Params);

	#if ENABLE_DRAW_DEBUG

		FVector TraceVec = GetActorForwardVector() * AttackRange;
		FVector Center = GetActorLocation() + TraceVec * 0.5f;
		float HalfHeight = AttackRange * 0.5f + AttackRadius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red; // ���������� �߻��ϸ� ������� ǥ��, �ƴϸ� ���������� ǥ��
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
		// UE4������ WeakPointer�� ��� �Ͽ��µ� UE5���� FActorInstanceHandle�� ������ ���� �Ǿ ��� �ڵ嵵 �����
			if (HitResult.GetActor()) // UE4������Actor.Isvalid() ���µ� UE5������ GetActor()�� �����
			{
				ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName()); // UE4������Actor->GetName() ���µ� UE5������ GetActor()->GetName()�� �����

				FDamageEvent DamageEvent; // �������� �����ϴ� �Լ� but  ���ظ� ���� ������ ���� ������ �������� ������ó���� ������
				HitResult.GetActor()->TakeDamage(50.0f, DamageEvent, GetController(), this); // �������� ����/ ������ ���� / ���ݸ���� ���� ������/ ������ ������ ���� ���� ����
			}
		}

}

float AABCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// TakeDamage �Լ��� �θ��� AActor �� ���������� �⺻ ������ �����Ǿ��־ super�� �θ��� ������ ���� ������������
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	ABLOG(Warning, TEXT("took Damage : %f"), FinalDamage); // *GetName ����� �ȵǴµ� ������ �ö󰡸鼭 ������ �ٲ��? ã�ƺ�����

	if (FinalDamage > 0.0f)
	{
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false); // �̰� �־�� ���� ��ü�� �浹�̺�Ʈ�� �߻���������
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