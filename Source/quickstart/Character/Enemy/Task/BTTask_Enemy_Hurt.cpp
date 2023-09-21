// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Enemy_Hurt.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "../EnemyController.h"
#include "AIController.h"
#include "GameFramework/Character.h"


UBTTask_Enemy_Hurt::UBTTask_Enemy_Hurt()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Enemy_Hurt, DetectionModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode")));
	CacheModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Enemy_Hurt, CacheModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode")));
}

EBTNodeResult::Type UBTTask_Enemy_Hurt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = OwnerComp.GetAIOwner();
	OwnerEnemy = Cast<AEnemy>(Controller->GetCharacter());

	auto Blackboard = Controller->GetBlackboardComponent();

	EEnemyDetectionMode Mode = EEnemyDetectionMode(Blackboard->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::HURT) return EBTNodeResult::Failed;	// 현재 감지모드가 Hurt가 아닐 경우 실패 처리

	Controller->StopMovement();	// 현재 이동 명령(MoveTo, Patrol 등)을 종료

	return EBTNodeResult::InProgress;
}

void UBTTask_Enemy_Hurt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto isEnded = !OwnerEnemy->GetMesh()->GetSingleNodeInstance()->IsPlaying();
	if (isEnded)	// Hurt 경직 애니메이션이 끝날 경우 태스크 종료
	{
		// DetectionMode를 CacheMode의 값으로 돌려놓음
		Controller->GetBlackboardComponent()->SetValueAsEnum(DetectionModeKey.SelectedKeyName, Controller->GetBlackboardComponent()->GetValueAsEnum(CacheModeKey.SelectedKeyName));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

