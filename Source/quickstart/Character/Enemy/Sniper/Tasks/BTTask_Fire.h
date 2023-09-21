// Behavior Tree Task - Fire
// Sniper 캐릭터의 공격 태스크
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

	// 타겟 액터의 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector Target;
	// Sniper의 현재 감지 모드
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
	// Target Actor
	AActor* TargetActor;
	// Task Owner
	ASniper* OwnerSniper;
	// Task Controller
	AAIController* Controller;
};
