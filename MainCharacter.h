#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class UAnimMontage;
class AEnemy;

UENUM(BlueprintType)
enum class EMovementState : uint8 {

	MS_Normal UMETA(DisplayName = "Normal"),
	MS_Sprinting UMETA(DisplayName = "Sprinting")

};

UCLASS()
class MONSTERKINGODTEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* FollowCamera;

	void MoveForward(float Value);
	void MoveRight(float Value);

	virtual void Jump() override;

	void SprintKeyUp();
	void SprintKeyDown();

	void LMBDown();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMovementState MovementState;

	UPROPERTY(BlueprintReadOnly)
		AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly)
		float MovementSpeed;

	UPROPERTY(BlueprintReadOnly)
		bool bInAir;

	void SetMovementSpeedAndAir();

	float RunningSpeed = 600.0f;
	float SprintingSpeed = 950.0f;

	void EquipWeapon(AWeapon* WeaponActor);

	UPROPERTY(EditAnywhere)
		UAnimMontage* CombatMontage;

	UFUNCTION()
		virtual void DamageBoxOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadWrite)
	bool CanDetectDamageCollision;

	UPROPERTY(BlueprintReadWrite)
		float Health = 100.0f;

	void ApplyDamage();

	void RestartGame();

};
