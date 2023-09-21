#include "BTTask_Bump.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../BigheadController.h"

UBTTask_Bump::UBTTask_Bump()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Bump, DetectionModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode"))); // Detection Mode�� ���� ����
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Bump, BlackboardKey), AActor::StaticClass()); // ���͸� ���� ����
}

EBTNodeResult::Type UBTTask_Bump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = OwnerComp.GetAIOwner();
	auto blackboard = Controller->GetBlackboardComponent();

	EEnemyDetectionMode Mode = EEnemyDetectionMode(blackboard->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::DETECTED) return EBTNodeResult::Failed;	// Detected ���°� �ƴϸ� ���� ���� X

	Target = Cast<AMainCharacter>(blackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));	// ���� Ÿ��
	if (Target)
	{
		OwnerBighead = Cast<ABighead>(Controller->GetCharacter());
		if (FVector::Dist2D(Target->GetActorLocation(), OwnerBighead->GetActorLocation()) <= AttackRange) // ���� ������ Ÿ���� ���Դ°�?
		{
			Impulse = (Target->GetActorLocation() - OwnerBighead->GetActorLocation()).GetSafeNormal2D() * 0.2f;	// ����� : Ÿ�� �������� �÷��̾ �з����� ����
			OwnerBighead->BumpAuraComponent->Activate(true); // ���� ȿ�� FX Ȱ��ȭ
			OwnerBighead->Bump(Target); // ������ ����
			return EBTNodeResult::InProgress; // TickTask�� ����
		}
	}

	return EBTNodeResult::Failed; // Ÿ���� ��ã�� ��� Fail
}

void UBTTask_Bump::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Target->GetCharacterMovement()->AddInputVector(Impulse); // ����ĸ� ĳ���Ϳ� ����
	if (!Target->bHurt) // Ÿ���� ���� ȿ���� �����°�?
	{
		OwnerBighead->BumpAuraComponent->Deactivate(); // ���� ȿ�� FX ��Ȱ��ȭ
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); // �½�ũ ����
	}
}