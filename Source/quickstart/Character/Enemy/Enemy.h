// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character_Root.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Components/WidgetComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "../../Object/Item/Bullet.h"
#include "../../UI/HPBar.h"
#include "Enemy.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEnemyDetectionMode : uint8
{
	PATROL,
	CAUTION,
	DETECTED,
	HURT,
	MAX,
};

UCLASS()
class QUICKSTART_API AEnemy : public ACharacter_Root
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION()
	void PlayDetectSound(bool isDoubt);

	virtual void OnHurt() override;

	virtual void OnDead() override;

public:

	UPROPERTY(BlueprintReadWrite)
	AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	UWidgetComponent* HPWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UHPBar> HPWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	UBillboardComponent* QuestionMarkComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard")
	UBillboardComponent* ExclamationMarkComponent;

	UAudioComponent* FireAudioComponent;
	UAudioComponent* DetectionAudioComponent;
	USoundCue* FireSound;
	USoundCue* DoubtingSound;
	USoundCue* DetectingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABullet> ProjectileClass;
	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float SightRadius = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float LoseSightRadius = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float PeripheralVisionAngleDegrees = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	FAISenseAffiliationFilter DetectionByAffiliation = FAISenseAffiliationFilter();
};
