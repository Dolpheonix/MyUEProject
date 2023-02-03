// Fill out your copyright notice in the Description page of Project Settings.


#include "ForceField.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Utils/Helpers.h"
#include "Components/BrushComponent.h"

AForceField::AForceField()
{
	PrimaryActorTick.bCanEverTick = true;
	GetBrushComponent()->SetCollisionProfileName("Volume");

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Helpers::SetComponent(&Arrow, GetBrushComponent(), FVector::ZeroVector, FRotator(90.0f, 0.0f, 0.0f));
}

void AForceField::BeginPlay()
{
	Super::BeginPlay();
	ForceDirection = Arrow->GetComponentRotation().Vector();
}

void AForceField::ActorEnteredVolume(AActor* other)
{
	Super::ActorEnteredVolume(other);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("ENTER??"));
	auto player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if ((AActor*)player == other)
	{
		isActivate = true;
	}
}

void AForceField::ActorLeavingVolume(AActor* other)
{
	Super::ActorLeavingVolume(other);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("LEAVE??"));

	auto player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if ((AActor*)player == other)
	{
		player->GetCharacterMovement()->GravityScale = 1.0f;
		isActivate = false;
	}
}

void AForceField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto player = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (isActivate)
	{
		float mass = player->GetCharacterMovement()->Mass;
		FVector Force = ForceDirection * 10000.0f * mass * ForceMagnitude;
		player->GetCharacterMovement()->GravityScale = 0.0f;
		player->GetCharacterMovement()->AddForce(Force);
	}
}