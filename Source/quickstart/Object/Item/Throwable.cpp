#include "Throwable.h"

AThrowable::AThrowable()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collision Component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->InitSphereRadius(13.0f);
	RootComponent = CollisionComponent;

	// �߻�ü Mesh Component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	MeshComponent->SetupAttachment(RootComponent);

	// �߻�ü Movement Component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 600.0f;
	ProjectileMovementComponent->MaxSpeed = 600.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;

	// �߻�ü ���� �ð�
	InitialLifeSpan = 5.0f;
}

void AThrowable::Throw(const FVector& Direction)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

