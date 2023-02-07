// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsObject.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Utils/Helpers.h"

// Sets default values
APhysicsObject::APhysicsObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	Helpers::SetComponent<UShapeComponent>(&CollisionComponent, nullptr, FVector::ZeroVector, FRotator::ZeroRotator);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Helpers::SetComponent<UStaticMeshComponent>(&MeshComponent, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator);

	UpdateCollisionSetting();
}

void APhysicsObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	auto propName = PropertyChangedEvent.MemberProperty->GetFName();

	if (propName == "Mesh" && Mesh)
	{
		MeshComponent->SetStaticMesh(Mesh);
	}
	else if (propName == "CollisionShape")
	{
		UpdateShape();
		UpdateCollisionSetting();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

// Called when the game starts or when spawned
void APhysicsObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APhysicsObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhysicsObject::UpdateShape()
{
	switch (CollisionShape)
	{
	case EShapeCollision::Box:
	{
		FVector origin = GetActorLocation();
		if (RootComponent) RootComponent->DestroyComponent(true);
		CollisionComponent = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("BoxComponent"));
		CollisionComponent->RegisterComponent();
		SetRootComponent(CollisionComponent);
		SetActorLocation(origin);
		FAttachmentTransformRules rule = { EAttachmentRule::KeepWorld, true };
		if (MeshComponent) MeshComponent->AttachToComponent(RootComponent, rule);
		break;
	}
	case EShapeCollision::Sphere:
	{
		FVector origin = GetActorLocation();
		if (RootComponent) RootComponent->DestroyComponent(true);
		CollisionComponent = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("SphereComponent"));
		CollisionComponent->RegisterComponent();
		SetRootComponent(CollisionComponent);
		SetActorLocation(origin);
		FAttachmentTransformRules rule = { EAttachmentRule::KeepWorld, true };
		if (MeshComponent) MeshComponent->AttachToComponent(RootComponent, rule);
		break;
	}
	case EShapeCollision::Capsule:
	{
		FVector origin = GetActorLocation();
		if (RootComponent) RootComponent->DestroyComponent(true);
		CollisionComponent = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("CapsuleComponent"));
		CollisionComponent->RegisterComponent();
		SetRootComponent(CollisionComponent);
		SetActorLocation(origin);
		FAttachmentTransformRules rule = { EAttachmentRule::KeepWorld, true };
		if (MeshComponent) MeshComponent->AttachToComponent(RootComponent, rule);
		break;
	}
	case EShapeCollision::Convex:
	{	
		FVector origin = GetActorLocation();
		if (RootComponent) RootComponent->DestroyComponent(true);
		USceneComponent* newRoot = NewObject<USceneComponent>(this, USceneComponent::StaticClass(), TEXT("SceneComponent"));
		newRoot->RegisterComponent();
		SetRootComponent(newRoot);
		SetActorLocation(origin);
		FAttachmentTransformRules rule = { EAttachmentRule::KeepWorld, true };
		if (MeshComponent) MeshComponent->AttachToComponent(RootComponent, rule);
		break;
	}
	default:
		break;
	}
}

void APhysicsObject::UpdateCollisionSetting()
{
	if (CollisionComponent == RootComponent)
	{
		CollisionComponent->SetSimulatePhysics(true);
		CollisionComponent->SetShouldUpdatePhysicsVolume(true);
		CollisionComponent->SetGenerateOverlapEvents(true);
		CollisionComponent->SetCollisionProfileName("PhysicsActor");

		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->SetShouldUpdatePhysicsVolume(false);
		MeshComponent->SetGenerateOverlapEvents(false);
		MeshComponent->SetCollisionProfileName("CharacterBody");
	}
	else
	{
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetShouldUpdatePhysicsVolume(true);
		MeshComponent->SetGenerateOverlapEvents(true);
		MeshComponent->SetCollisionProfileName("PhysicsActor");
	}
}
