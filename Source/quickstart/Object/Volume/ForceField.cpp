// Fill out your copyright notice in the Description page of Project Settings.


#include "ForceField.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BrushComponent.h"

AForceField::AForceField()
{
	GetBrushComponent()->SetCollisionProfileName("Volume");
}

void AForceField::ActorEnteredVolume(AActor* other)
{
	Super::ActorEnteredVolume(other);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("ENTER??"));
}

void AForceField::ActorLeavingVolume(AActor* other)
{
	Super::ActorLeavingVolume(other);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("LEAVE??"));
}