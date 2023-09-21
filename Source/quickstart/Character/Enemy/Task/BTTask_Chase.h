// Behavior Tree Task - Chase
// 타겟을 쫒아가는 MoveTo 태스크를 상속
// 감지 모드에 따라 태스크 중단 여부를 결정하도록 기능을 추가
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_Chase.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_Chase : public UBTTask_MoveTo
{
	GENERATED_BODY()

	UBTTask_Chase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 현재 감지 모드
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
};
