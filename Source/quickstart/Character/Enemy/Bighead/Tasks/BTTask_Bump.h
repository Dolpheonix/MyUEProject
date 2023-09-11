// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../Bighead.h"
#include "../../../Main/MainCharacter.h"
#include "BTTask_Bump.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_Bump : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UBTTask_Bump();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category="BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;

	AAIController* Controller;
	ABighead* OwnerBighead;
	AMainCharacter* Target;
	FVector Impulse;

	float elapsed = 0.0f;
};
