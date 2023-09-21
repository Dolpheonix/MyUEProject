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
	if (currMode != EEnemyDetectionMode::CAUTION) return EBTNodeResult::Failed;	// ���� ������尡 Caution�� �ƴ� ��� ���� ó��

	OwnerComp.GetAIOwner()->StopMovement();	// ���� �̵� ����� �ߴ�

	return EBTNodeResult::InProgress;
}

void UBTTask_StareAt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	EEnemyDetectionMode currMode = EEnemyDetectionMode(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (currMode != EEnemyDetectionMode::CAUTION)	// Caution ���¸� ����� (Detected / Patrol / Hurt) �½�ũ ���� ó��
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

