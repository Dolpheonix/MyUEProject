// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_NPC_Stroll.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_NPC_Stroll::UBTTask_NPC_Stroll()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	InteractKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_NPC_Stroll, InteractKey));
}

EBTNodeResult::Type UBTTask_NPC_Stroll::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto res = Super::ExecuteTask(OwnerComp, NodeMemory);

	bool interacted = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool(InteractKey.SelectedKeyName);
	if (interacted) return EBTNodeResult::Failed;
	return res;
}

void UBTTask_NPC_Stroll::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool(InteractKey.SelectedKeyName))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}