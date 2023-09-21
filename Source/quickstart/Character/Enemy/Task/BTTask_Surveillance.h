// Behavior Tree Task - Surveillance
// 특정 위치로 이동하는 태스크
// 이동 중 적이 감지되면 태스크 실패 처리

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_Surveillance.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_Surveillance : public UBTTask_MoveTo
{
	GENERATED_BODY()
	UBTTask_Surveillance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 현재 감지 모드
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
};
