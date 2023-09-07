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

	virtual void OnHurt() override;

	virtual void OnDead() override;

	UFUNCTION()
	void PlayDetectSound(bool isDoubt);
public:

	UPROPERTY(BlueprintReadWrite)
	AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassSelection")
	TSubclassOf<UHPBar> HPWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	UBillboardComponent* QuestionMarkComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard")
	UBillboardComponent* ExclamationMarkComponent;

	UWidgetComponent* HPWidget;
	UAudioComponent* DetectionAudioComponent;
	USoundCue* DoubtingSound;
	USoundCue* DetectingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float SightRadius = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float LoseSightRadius = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float PeripheralVisionAngleDegrees = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	FAISenseAffiliationFilter DetectionByAffiliation = FAISenseAffiliationFilter();
};
