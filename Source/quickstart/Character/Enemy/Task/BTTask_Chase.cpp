// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Chase.h"
#include "AIController.h"
#include "../EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Chase::UBTTask_Chase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Chase, DetectionModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode")));
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	EEnemyDetectionMode Mode = EEnemyDetectionMode(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::DETECTED) return EBTNodeResult::Failed;	// 현재 감지 모드가 Detected가 아닐 경우 실패 처리


	return result;
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	EEnemyDetectionMode Mode = EEnemyDetectionMode(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::DETECTED)	// 감지 모드가 Detected를 벗어날 경우 중도 실패 처리
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

