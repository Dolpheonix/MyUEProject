// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../../Utils/NPCUtil.h"
#include "Beacon.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API ABeacon : public AActor
{
	GENERATED_BODY()
	
public:
	ABeacon();

	UFUNCTION()
	void OnOverlapped_Arrival(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void RegisterQuest(FSingleQuest* quest);

public:
	UBoxComponent* Volume;
	UParticleSystemComponent* FXComponent;
	FSingleQuest* Ownerquest;
};
