// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/ArrowComponent.h"
#include "../WorldObject/PhysicsObject.h"
#include "../../Character/Character_Root.h"
#include "ForceField.generated.h"

/**
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct FEnterForceFieldForm_Character
{
	GENERATED_USTRUCT_BODY()

	ACharacter_Root* EnteredActor;
	FVector InitialVelocity;

	FEnterForceFieldForm_Character() : EnteredActor(nullptr), InitialVelocity(FVector::ZeroVector) {}
	FEnterForceFieldForm_Character(ACharacter_Root* actor, FVector vel) : EnteredActor(actor), InitialVelocity(vel) {}
};

USTRUCT(Atomic, BlueprintType)
struct FEnterForceFieldForm_Object
{
	GENERATED_USTRUCT_BODY()

	APhysicsObject* EnteredActor;
	FVector InitialVelocity;

	FEnterForceFieldForm_Object() : EnteredActor(nullptr), InitialVelocity(FVector::ZeroVector) {}
	FEnterForceFieldForm_Object(APhysicsObject* actor, FVector vel) : EnteredActor(actor), InitialVelocity(vel) {}
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
	UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, Category="Test")
	TArray<FEnterForceFieldForm_Object> ActorEntry;

	UPROPERTY()
	TArray<FEnterForceFieldForm_Character> CharacterEntry;

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