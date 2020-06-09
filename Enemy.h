#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class USphereComponent;
class UBoxComponent;
class AMainCharacter;
class UAnimMontage;
class AAIController;


UCLASS()
class MONSTERKINGODTEST_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		USphereComponent* DetectSphere;

	UPROPERTY(EditAnywhere)
		USphereComponent* CombatSphere;

	UPROPERTY(EditAnywhere)
		UBoxComponent* AttackHitBox;

	UFUNCTION()
		virtual void DetectSphereOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void DetectSphereOnEndOverlap(UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void CombatSphereOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void CombatSphereOnEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void AttackHitBoxOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);


	AMainCharacter* TargetChar;

	AAIController* AI_Controller;

	void MoveToTarget();

	UPROPERTY(BlueprintReadOnly)
	bool IsAlive = true;

	bool bAttacking;

	UPROPERTY(EditAnywhere)
		float MoveToTargetRadius = 10.0f;

	UPROPERTY(EditAnywhere)
		bool bTargetInAttackRange;

	float AttackDelayMin = 0.2f;
	float AttackDelayMax = 0.7f;

	bool CanDetectDamageCollision;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnded();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere)
		UAnimMontage* CombatMontage;

	UPROPERTY(BlueprintReadWrite)
		float Health = 100.0f;

	void ApplyDamage();
	void DisposeEnemy();
};
