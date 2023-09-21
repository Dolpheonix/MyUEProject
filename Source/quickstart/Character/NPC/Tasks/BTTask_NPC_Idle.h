// Behavior Tree Task - NPC Idle
// NPC의 Wait 태스크
// 플레이어가 상호작용할 시 실패 처리
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_NPC_Idle.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_NPC_Idle : public UBTTask_Wait
{
	GENERATED_BODY()
	
	UBTTask_NPC_Idle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 상호작용 여부를 확인하는 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector InteractKey;
};
