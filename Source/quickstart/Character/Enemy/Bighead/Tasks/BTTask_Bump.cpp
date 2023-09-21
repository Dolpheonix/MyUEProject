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

	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Bump, DetectionModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode"))); // Detection Mode만 설정 가능
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Bump, BlackboardKey), AActor::StaticClass()); // 액터만 설정 가능
}

EBTNodeResult::Type UBTTask_Bump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = OwnerComp.GetAIOwner();
	auto blackboard = Controller->GetBlackboardComponent();

	EEnemyDetectionMode Mode = EEnemyDetectionMode(blackboard->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::DETECTED) return EBTNodeResult::Failed;	// Detected 상태가 아니면 공격 실행 X

	Target = Cast<AMainCharacter>(blackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));	// 공격 타겟
	if (Target)
	{
		OwnerBighead = Cast<ABighead>(Controller->GetCharacter());
		if (FVector::Dist2D(Target->GetActorLocation(), OwnerBighead->GetActorLocation()) <= AttackRange) // 공격 범위에 타겟이 들어왔는가?
		{
			Impulse = (Target->GetActorLocation() - OwnerBighead->GetActorLocation()).GetSafeNormal2D() * 0.2f;	// 충격파 : 타겟 뒤쪽으로 플레이어가 밀려나는 방향
			OwnerBighead->BumpAuraComponent->Activate(true); // 공격 효과 FX 활성화
			OwnerBighead->Bump(Target); // 데미지 적용
			return EBTNodeResult::InProgress; // TickTask로 진행
		}
	}

	return EBTNodeResult::Failed; // 타겟을 못찾을 경우 Fail
}

void UBTTask_Bump::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Target->GetCharacterMovement()->AddInputVector(Impulse); // 충격파를 캐릭터에 적용
	if (!Target->bHurt) // 타겟의 경직 효과가 끝났는가?
	{
		OwnerBighead->BumpAuraComponent->Deactivate(); // 공격 효과 FX 비활성화
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); // 태스크 성공
	}
}