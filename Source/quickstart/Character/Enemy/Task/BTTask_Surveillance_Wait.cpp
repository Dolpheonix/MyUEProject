#include "BTTask_Surveillance_Wait.h"
#include "AIController.h"
#include "../EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Surveillance_Wait::UBTTask_Surveillance_Wait(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Surveillance_Wait, DetectionModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode")));
}

EBTNodeResult::Type UBTTask_Surveillance_Wait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return result;
}

void UBTTask_Surveillance_Wait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	EEnemyDetectionMode Mode = EEnemyDetectionMode(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::PATROL)	// 현재 감지 모드가 Patrol에서 벗어날 경우 태스크 실패 처리
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}