// Behavior Tree Task - Fire
// Sniper ĳ������ ���� �½�ũ
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "../Sniper.h"
#include "BTTask_Fire.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_Fire : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_Fire();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:

	// Ÿ�� ������ ������ Ű
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector Target;
	// Sniper�� ���� ���� ���
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
	// Target Actor
	AActor* TargetActor;
	// Task Owner
	ASniper* OwnerSniper;
	// Task Controller
	AAIController* Controller;
};
