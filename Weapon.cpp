#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

#include "MainCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	AttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox"));
	AttackHitBox->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate(DeltaTime);

}

void AWeapon::Rotate(float DeltaTime)
{
	if (Used)
		return;

	auto Rotation = GetActorRotation();
	Rotation.Yaw += DeltaTime * RotationRate;
	SetActorRotation(Rotation);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player")) {
		
		AMainCharacter* PlayerChar = Cast<AMainCharacter>(OtherActor);
		PlayerChar->EquipWeapon(this);
	}

}






