// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interactable.h"
#include "CollectableItem.generated.h"

/*
 If player interact with this actor, actor destroyed. But class of this item is added to player's inventory.
 */
UCLASS()
class QUICKSTART_API ACollectableItem : public AInteractable
{
	GENERATED_BODY()
	
public:
	ACollectableItem();

	virtual void BeginPlay() override;
	virtual void Interact() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* ItemMesh;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* AuraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
	ETypeTag TypeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
	FString NameTag;

	FString InfoTag;
	int32 WeaponCode;
};
