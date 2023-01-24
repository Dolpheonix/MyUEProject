// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwable.h"

// Sets default values
AThrowable::AThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �߻�ü collision ����
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AThrowable::OnHit); // component�� hit event�� �߻����� ��, OnHit function�� ȣ���ϵ��� binding
	CollisionComponent->InitSphereRadius(13.0f);
	RootComponent = CollisionComponent;

	// �߻�ü mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	MeshComponent->SetupAttachment(RootComponent);

	// �߻�ü movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 600.0f;
	ProjectileMovementComponent->MaxSpeed = 600.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // velocity�� ���⿡ ���� �߻�ü�� ȸ���ϰ� ��
	ProjectileMovementComponent->bShouldBounce = true; // �߻�ü�� bounce�� ����
	ProjectileMovementComponent->Bounciness = 0.3f; // bounce �Ŀ� �����Ǵ� velocity

	// �߻�ü ���� �ð�
	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void AThrowable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThrowable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowable::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

void AThrowable::Throw(const FVector& Direction)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

