#include "MainCharacter.h"

void AMainCharacter::NoItem()
{
}

void AMainCharacter::ThrowApple()
{
	FVector ThrowPoint = GetActorLocation() + FVector(0.0f, 0.0f, 60.0f) + 15.0f * GetActorForwardVector();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AThrowable* Apple = GetWorld()->SpawnActor<AThrowable>(AThrowable::StaticClass(), ThrowPoint, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	if (Apple)
	{
		Apple->Tag = "Apple";
		Apple->CollisionComponent->InitSphereRadius(13.0f);
		Apple->MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Item/StaticMesh/Apple.Apple")));
		Apple->MeshComponent->SetRelativeScale3D(FVector(13.0f, 13.0f, 13.0f));
		Apple->MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -52.657f));
		Apple->MeshComponent->SetCollisionProfileName("Projectile");
		FVector ThrowDirection = GetActorForwardVector();
		Apple->Throw(ThrowDirection);
	}
}

void AMainCharacter::ThrowSeed()
{
	FVector ThrowPoint = GetActorLocation() + FVector(0.0f, 0.0f, 60.0f) + 15.0f * GetActorForwardVector();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AThrowable* Seed = GetWorld()->SpawnActor<AThrowable>(AThrowable::StaticClass(), ThrowPoint, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	if (Seed)
	{
		Seed->Tag = "Seed";
		Seed->CollisionComponent->InitSphereRadius(13.0f);
		Seed->CollisionComponent->SetCollisionProfileName("Projectile");
		Seed->MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Item/StaticMesh/Seed.Seed")));
		Seed->MeshComponent->SetRelativeScale3D(FVector(8.0f, 8.0f, 8.0f));
		FVector ThrowDirection = GetActorForwardVector();
		Seed->Throw(ThrowDirection);
	}
}