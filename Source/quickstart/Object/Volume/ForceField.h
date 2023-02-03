// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/ArrowComponent.h"
#include "ForceField.generated.h"

/**
 * 
 */
UCLASS(hidecategories = (CharacterMovement))
class QUICKSTART_API AForceField : public APhysicsVolume
{
	GENERATED_BODY()

public:
	AForceField();

	virtual void ActorEnteredVolume(class AActor* Other) override;

	virtual void ActorLeavingVolume(class AActor* Other) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
public:
	bool isActivate = false;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* Arrow;

	FVector ForceDirection;

	UPROPERTY(EditAnywhere, Category = "Force")
	float ForceMagnitude = 1.0f;
};
