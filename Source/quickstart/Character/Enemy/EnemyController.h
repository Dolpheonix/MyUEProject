// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName PlayerKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName OriginLocationKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName HurtKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBehaviorTree* BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBlackboardData* BB;
};
