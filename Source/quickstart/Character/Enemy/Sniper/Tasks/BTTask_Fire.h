// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "../Sniper.h"
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

	// 타겟 오브젝트
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector Target;
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;

	float MuzzlePoint;
	AActor* TargetActor;
	ASniper* OwnerSniper;
	AAIController* Controller;
};
