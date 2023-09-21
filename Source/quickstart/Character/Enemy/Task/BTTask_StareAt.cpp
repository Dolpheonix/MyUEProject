#include "BTTask_StareAt.h"
#include "../EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_StareAt::UBTTask_StareAt(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	UEnum* enumClass = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode"));
	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_StareAt, DetectionModeKey), enumClass);
}

EBTNodeResult::Type UBTTask_StareAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EEnemyDetectionMode currMode = EEnemyDetectionMode(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (currMode != EEnemyDetectionMode::CAUTION) return EBTNodeResult::Failed;	// 현재 감지모드가 Caution이 아닐 경우 실패 처리

	OwnerComp.GetAIOwner()->StopMovement();	// 현재 이동 명령을 중단

	return EBTNodeResult::InProgress;
}

void UBTTask_StareAt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	EEnemyDetectionMode currMode = EEnemyDetectionMode(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (currMode != EEnemyDetectionMode::CAUTION)	// Caution 상태를 벗어나면 (Detected / Patrol / Hurt) 태스크 실패 처리
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

