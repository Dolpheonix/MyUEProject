// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "../Enemy.h"
#include "BTTask_Fire.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_Fire : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_Fire();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector Target;

	float MuzzlePoint;
	AActor* TargetActor;
	AEnemy* OwnerEnemy;
	AAIController* Controller;
};
