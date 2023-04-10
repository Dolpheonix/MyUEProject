// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_Move.h"
#include "BTTask_Return.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_Return : public UBTTask_Move
{
	GENERATED_BODY()

	UBTTask_Return();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// When Path between MoniterTarget ~ SelfActor exist, Task will be failed
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector MoniterTarget;

	AActor* MoniterTargetActor;
};
