// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enemy.h"
#include "EnemyController.generated.h"

/**
 * 
 */


UCLASS()
class QUICKSTART_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName PlayerKeyName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName OriginLocationKeyName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName HurtKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBehaviorTree* BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBlackboardData* BB;

	AEnemy* OwnerEnemy;
	APawn* MainPlayer;

	class UAISenseConfig_Sight* SightSense;

	FTimerHandle CautionToDetectedTimer;
	FTimerHandle TargetHoldTimer;


	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* SourceActor, FAIStimulus Stimulus);
	UFUNCTION()
	void CautionToDetected();
	UFUNCTION()
	void IsTargetValid();

	TArray<AActor*> PerceivedList;
};
