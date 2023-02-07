// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/ArrowComponent.h"
#include "../WorldObject/PhysicsObject.h"
#include "ForceField.generated.h"

/**
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct FEnterForceFieldForm
{
	GENERATED_USTRUCT_BODY()

	APhysicsObject* EnteredActor;
	FVector InitialVelocity;

	FEnterForceFieldForm() : EnteredActor(nullptr), InitialVelocity(FVector::ZeroVector) {}
	FEnterForceFieldForm(APhysicsObject* actor, FVector vel) : EnteredActor(actor), InitialVelocity(vel) {}
};

UCLASS(hidecategories = (CharacterMovement))
class QUICKSTART_API AForceField : public APhysicsVolume
{
	GENERATED_BODY()

public:
	AForceField();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void ActorEnteredVolume(class AActor* Other) override;

	virtual void ActorLeavingVolume(class AActor* Other) override;

public:
	bool isActivate = false;

	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, Category="Test")
	TArray<FEnterForceFieldForm> ActorEntry;

	// Direction of Force. Relative to Actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force")
	FRotator ForceDirection = FRotator::ZeroRotator;

	// Magnitude of Force. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float ForceMagnitude = 1.f;

	// Damping scale of Initial velocity of entered object.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float DampingScale = 1.f;

	FVector ForceVector = FVector::ZeroVector;
};