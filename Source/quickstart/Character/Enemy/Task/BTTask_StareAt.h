// Behavior Tree Task - Stare At
// 타겟을 응시하는 태스크
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StareAt.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_StareAt : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_StareAt(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 현재 감지 모드
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
};
