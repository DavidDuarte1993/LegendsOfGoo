#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class MONSTERKINGODTEST_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	void Rotate(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotationRate = 45.0f;

	bool Used;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* AttackHitBox;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
			bool bFromSweep, const FHitResult& SweepResult);

};
