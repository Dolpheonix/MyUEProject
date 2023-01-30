// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Portal.generated.h"

UCLASS()
class QUICKSTART_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	void OnOverlapped_Teleport(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void UpdatePair(APortal* newpair);
	void Connect();
	void Disconnect();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* Volume;

	UPROPERTY(EdItAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* FXComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	UParticleSystem* PortalFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	bool bPairing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal", meta = (EditCondition = "bPairing"))
	APortal* Pair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* Destination;

	APortal* Oldpair;
};

