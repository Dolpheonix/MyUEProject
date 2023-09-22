#include "Bullet.h"
#include "../../Utils/Helpers.h"
#include "../../Character/Enemy/Enemy.h"
#include "../../Character/Character_Root.h"
#include "Kismet/GameplayStatics.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collision Component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapped);
	CollisionComponent->InitSphereRadius(3.3f);
	RootComponent = CollisionComponent;

	// �߻�ü Mesh Component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Primitive/StaticMesh/Sphere.Sphere")));
	ProjectileMesh->SetMaterial(0, Helpers::C_LoadObjectFromPath<UMaterial>(TEXT("/Game/ShootingGame/Asset/Material/M_Metal_Steel.M_Metal_Steel")));
	Helpers::SetComponent(&ProjectileMesh, RootComponent, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.f), FVector(0.02f, 0.02f, 0.02f));
	ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));

	// �߻�ü Movement Component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // velocity�� ���⿡ ���� �߻�ü�� ȸ���ϰ� ��
	ProjectileMovementComponent->bShouldBounce = true; // �߻�ü�� ���� ���� �� Bounciness��ŭ �ӷ��� ������
	ProjectileMovementComponent->Bounciness = 0.3f;

	// �߻�ü ���� �ð�
	InitialLifeSpan = 3.0f;
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}

	Destroy();
}

void ABullet::OnOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Hittee = Cast<ACharacter_Root>(OtherActor);
	if (Hittee)
	{
		UGameplayStatics::ApplyDamage(Hittee, BulletDamage, nullptr, this, NULL); // �������� ����
	}

	Destroy();
}

void ABullet::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
