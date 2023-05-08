// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../Enemy.h"
#include "BTTask_Enemy_Hurt.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_Enemy_Hurt : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_Enemy_Hurt();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector CacheModeKey;

	AEnemy* OwnerEnemy;
	AAIController* Controller;
};
