// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PreviewActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowroomEvent);

UCLASS()
class QUICKSTART_API APreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreviewActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void Sync_to_Character();

	UFUNCTION()
	void Delete_and_Sync(ETypeTag type, FString tag);

	FShowroomEvent SyncEvent;



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	USkeletalMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	USceneCaptureComponent2D* PreviewCamera;

	UPROPERTY(BlueprintReadWrite)
	int Weapon_Now;

	UPROPERTY(BlueprintReadOnly)
	int32 WeaponCode;

	int Cloth_Now;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	TMap<FString, UStaticMeshComponent*> PreviewWeapons;
	TMap<FString, UStaticMeshComponent*> PreviewClothes;

	AMainCharacter* Player;
};
