// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "../../Utils/Helpers.h"
#include "../../Character/Enemy/Enemy.h"
#include "../../Character/Character_Root.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �߻�ü collision ����
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit); // component�� hit event�� �߻����� ��, OnHit function�� ȣ���ϵ��� binding
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapped);
	CollisionComponent->InitSphereRadius(3.3f);
	RootComponent = CollisionComponent;

	// �߻�ü mesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Sphere.Sphere")));
	ProjectileMesh->SetMaterial(0, Helpers::C_LoadObjectFromPath<UMaterial>(TEXT("/Game/ShootingGame/Material/M_Metal_Steel.M_Metal_Steel")));
	Helpers::SetComponent(&ProjectileMesh, RootComponent, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.f), FVector(0.02f, 0.02f, 0.02f));
	ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));

	// �߻�ü movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // velocity�� ���⿡ ���� �߻�ü�� ȸ���ϰ� ��
	ProjectileMovementComponent->bShouldBounce = true; // �߻�ü�� bounce�� ����
	ProjectileMovementComponent->Bounciness = 0.3f; // bounce �Ŀ� �����Ǵ� velocity

	// �߻�ü ���� �ð�
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// HitComponent : �� Actor ������ collision�� �߻��� component (���⼱ spherecomponent)
// OtherActor : collision�� �߻��� �ݴ��� Actor
// OtherComponent : collision�� �߻��� �ݴ��� component
// NormalImpulse : impulse vector 
void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) // other object�� physics�� �����ϴ� ���, hitting result�� �����ؾ� ��
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
		UGameplayStatics::ApplyDamage(Hittee, BulletDamage, nullptr, this, NULL);
	}

	Destroy();
}


// projectile�� velocity�� ����
void ABullet::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
