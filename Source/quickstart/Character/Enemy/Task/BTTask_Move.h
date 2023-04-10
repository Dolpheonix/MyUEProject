// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BTTask_Move.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_Move : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:	
	UBTTask_Move();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category="Target")
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere, Category="Radius")
	float Radius;

	bool bFollowActor = false;

	AActor* TargetActor;
	FVector TargetVector;

	ACharacter* OwnerActor;
	AAIController* Controller;
	UBlackboardComponent* Blackboard;
};
