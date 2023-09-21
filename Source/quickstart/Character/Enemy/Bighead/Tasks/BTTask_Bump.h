// UBTTask_Bump
// ABighead의 공격 태스크

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../Bighead.h"
#include "../../../Main/MainCharacter.h"
#include "BTTask_Bump.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_Bump : public UBTTask_BlackboardBase	// UBTTask_BlackboardBase의 Blackboard Key : 공격 타겟
{
	GENERATED_BODY()
	
	UBTTask_Bump();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// 현재 Enemy의 Detection Mode
	UPROPERTY(EditAnywhere, Category="BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
	// 공격 범위
	UPROPERTY(EditAnywhere, Category="Range")
	float AttackRange = 30.0f;
	// AI 컨트롤러
	AAIController* Controller;
	// 태스크를 실행중인 액터
	ABighead* OwnerBighead;
	// 공격 타겟 (메인 캐릭터)
	AMainCharacter* Target;
	// 타겟에 줄 충격파 벡터
	FVector Impulse;
};
