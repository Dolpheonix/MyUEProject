#include "BTTask_Fire.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "../SniperController.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTTask_Fire::UBTTask_Fire()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Fire, DetectionModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode")));
	Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Fire, Target), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = OwnerComp.GetAIOwner();
	OwnerSniper = Cast<ASniper>(Controller->GetCharacter());

	auto Blackboard = Controller->GetBlackboardComponent();
	TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(Target.SelectedKeyName));

	EEnemyDetectionMode Mode = EEnemyDetectionMode(Blackboard->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::DETECTED) return EBTNodeResult::Failed;	// 현재 감지 모드가 Detected가 아닐 경우 태스크 실패 처리

	FVector dir = (TargetActor->GetActorLocation() - OwnerSniper->GetActorLocation()).GetSafeNormal();	// 오너 -> 타겟
	FRotator rot = FRotationMatrix::MakeFromX(dir).Rotator();	// Rotator

	OwnerSniper->SetActorRotation(rot);	// 타겟 방향으로 몸을 돌림

	OwnerSniper->Fire();	// 공격

	return EBTNodeResult::InProgress;
}

void UBTTask_Fire::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto isEnded = !OwnerSniper->GetMesh()->GetSingleNodeInstance()->IsPlaying();
	EEnemyDetectionMode Mode = EEnemyDetectionMode(Controller->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (isEnded || Mode != EEnemyDetectionMode::DETECTED)	// 공격(발포) 애니메이션이 끝나거나, 감지 모드가 변경됐을 경우 종료
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
