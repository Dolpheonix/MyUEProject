// Behavior Tree Task - Surveillance Wait
// 일정 시간동안 멈춰 있는 태스크
// 감지 모드가 변경되면 태스크 실패
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_Surveillance_Wait.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_Surveillance_Wait : public UBTTask_Wait
{
	GENERATED_BODY()
	UBTTask_Surveillance_Wait(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 현재 감지 모드
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
};
