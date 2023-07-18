// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Interactable_LevelEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractableLevelEvent);

/**
 * 
 */
UCLASS()
class QUICKSTART_API AInteractable_LevelEvent : public AInteractable
{
	GENERATED_BODY()
	
public:
	AInteractable_LevelEvent();
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FInteractableLevelEvent LevelEvent;

	virtual void Interact() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MainMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* MainMesh;
};
