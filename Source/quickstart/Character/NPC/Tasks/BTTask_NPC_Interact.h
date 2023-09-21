// Behavior Tree Task - NPC Interact
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NPC_Interact.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_NPC_Interact : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_NPC_Interact();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 상호작용 여부를 확인하는 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector InteractKey;
	// 상호작용 중인 타겟(플레이어)
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector Target;
};
