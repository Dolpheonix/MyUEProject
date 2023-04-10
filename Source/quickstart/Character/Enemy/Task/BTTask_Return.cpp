// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Return.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/Character.h"
#include "../../../Utils/AIUtil.h"

UBTTask_Return::UBTTask_Return()
{
	bNotifyTick = true;
	bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTTask_Return::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto res = UBTTask_Move::ExecuteTask(OwnerComp, NodeMemory);

	auto BlackboardData = GetBlackboardAsset();
	MoniterTarget.ResolveSelectedKey(*BlackboardData);
	if (MoniterTarget.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		MoniterTargetActor = Cast<AActor>(Blackboard->GetValueAsObject(MoniterTarget.SelectedKeyName));
	}
	else return EBTNodeResult::Failed;

	return res;
}

void UBTTask_Return::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBTTask_Move::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FVector OwnerLoc = OwnerActor->GetActorLocation();
	FVector TargetLoc = MoniterTargetActor->GetActorLocation();
	if (AIHelper::isValidPath(GetWorld(), OwnerLoc, TargetLoc))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
