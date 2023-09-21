#include "BTTask_Surveillance.h"
#include "AIController.h"
#include "../EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Surveillance::UBTTask_Surveillance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Surveillance, DetectionModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode")));
}

EBTNodeResult::Type UBTTask_Surveillance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EEnemyDetectionMode Mode = EEnemyDetectionMode(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::PATROL)	// 현재 감지 모드가 Patrol이 아닐 경우 태스크 실패 처리
	{
		return EBTNodeResult::Failed;
	}

	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	return result;
}

void UBTTask_Surveillance::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	EEnemyDetectionMode Mode = EEnemyDetectionMode(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::PATROL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}