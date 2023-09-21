// Behavior Tree Task - Enemy Hurt
// Enemy 캐릭터가 데미지를 입을 시 실행되는 태스크

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../Enemy.h"
#include "BTTask_Enemy_Hurt.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_Enemy_Hurt : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_Enemy_Hurt();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 현재 감지 모드
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
	// 데미지를 입어 Hurt 상태가 되기 전, 마지막의 감지 모드
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector CacheModeKey;

	AEnemy* OwnerEnemy;
	AAIController* Controller;
};
