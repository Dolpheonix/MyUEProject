// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"
#include "../../Utils/Structs.h"
#include "PhysicsObject.generated.h"

UCLASS(hidecategories="Components")
class QUICKSTART_API APhysicsObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsObject();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void UpdateShape();
	void UpdateCollisionSetting();
	inline UPrimitiveComponent* GetPhysicsComponent() { if (CollisionComponent) return CollisionComponent; else return MeshComponent; };

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UShapeComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = "Shape")
	EShapeCollision CollisionShape = EShapeCollision::Box;
};
