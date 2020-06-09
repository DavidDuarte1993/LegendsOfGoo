#include "MainCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"

#include "Weapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 550.0f;
	GetCharacterMovement()->AirControl = 0.2f;

}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	

}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetMovementSpeedAndAir();

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::SprintKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::SprintKeyUp);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainCharacter::LMBDown);

}

void AMainCharacter::MoveForward(float Value)
{

	if (!IsAlive)
		return;

	if (Value != 0.0f && !bIsAttacking) {
	
		auto YawRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);

	}

}

void AMainCharacter::MoveRight(float Value)
{

	if (!IsAlive)
		return;

	if (Value != 0.0f && !bIsAttacking) {

		auto YawRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);

	}

}

void AMainCharacter::Jump()
{

	if (!IsAlive)
		return;

	if (bIsAttacking)
		return;

	Super::Jump();

}

void AMainCharacter::SprintKeyDown()
{

	if (!IsAlive)
		return;

	if (GetCharacterMovement()->Velocity.Size() == 0)
		return;

	MovementState = EMovementState::MS_Sprinting;
	GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;

}

void AMainCharacter::SprintKeyUp()
{

	if (!IsAlive)
		return;

	MovementState = EMovementState::MS_Normal;
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;

}

void AMainCharacter::LMBDown()
{

	if (!IsAlive)
		return;

	if (EquippedWeapon) {
		
		UAnimInstance* MyAnim = GetMesh()->GetAnimInstance();

		if (MyAnim && CombatMontage) {


			MyAnim->Montage_Play(CombatMontage, 2.5f);

			float RandAnim = FMath::RandRange(0, 1);

			if (RandAnim > 0) {

				MyAnim->Montage_JumpToSection(TEXT("Attack_1"));
			
			}
			else {
			
				MyAnim->Montage_JumpToSection(TEXT("Attack_2"));

			}

			CanDetectDamageCollision = true;

		}
	}
}

void AMainCharacter::SetMovementSpeedAndAir()
{

	auto CharSpeed = GetVelocity();
	auto LateralSpeed = FVector(CharSpeed.X, CharSpeed.Y, 0.0f);

	MovementSpeed = LateralSpeed.Size();

	bInAir = GetMovementComponent()->IsFalling();

}

void AMainCharacter::EquipWeapon(AWeapon * WeaponActor)
{

	if (EquippedWeapon != nullptr)
		return;

	WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
		TEXT("RightHandSocket"));

	WeaponActor->Used = true;
	EquippedWeapon = WeaponActor;

	EquippedWeapon->AttackHitBox->OnComponentBeginOverlap.AddDynamic(this, 
		&AMainCharacter::DamageBoxOnBeginOverlap);

}

void AMainCharacter::DamageBoxOnBeginOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (CanDetectDamageCollision) {
	
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy) {
		
			CanDetectDamageCollision = false;

			Enemy->ApplyDamage();
		}
	}

}

void AMainCharacter::ApplyDamage()
{

	if (!IsAlive)
		return;

	Health -= 10.0f;

	if (Health <= 0.0f) {
	
		IsAlive = false;

		FTimerHandle TimerHandler;
		GetWorldTimerManager().SetTimer(TimerHandler, this, &AMainCharacter::RestartGame, 3.0f);

	}

}

void AMainCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}




