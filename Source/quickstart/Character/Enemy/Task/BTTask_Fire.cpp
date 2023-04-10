// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "GameFramework/Character.h"

UBTTask_Fire::UBTTask_Fire()
{
	bNotifyTick = true;
	bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = OwnerComp.GetAIOwner();
	OwnerEnemy = Cast<AEnemy>(Controller->GetCharacter());

	auto Blackboard = Controller->GetBlackboardComponent();
	TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(Target.SelectedKeyName));

	OwnerEnemy->Fire();

	return EBTNodeResult::InProgress;
}

void UBTTask_Fire::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto isEnded = !OwnerEnemy->GetMesh()->GetSingleNodeInstance()->IsPlaying();
	if (isEnded) FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
