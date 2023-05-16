// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NPC_Interact.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_NPC_Interact : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_NPC_Interact();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector InteractKey;

	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector Target;
};
