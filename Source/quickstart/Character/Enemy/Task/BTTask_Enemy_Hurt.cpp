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
	if (Mode != EEnemyDetectionMode::HURT) return EBTNodeResult::Failed;

	Controller->StopMovement();

	return EBTNodeResult::InProgress;
}

void UBTTask_Enemy_Hurt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto isEnded = !OwnerEnemy->GetMesh()->GetSingleNodeInstance()->IsPlaying();
	if (isEnded)
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum(DetectionModeKey.SelectedKeyName, Controller->GetBlackboardComponent()->GetValueAsEnum(CacheModeKey.SelectedKeyName));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

