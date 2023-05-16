// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_NPC_Interact.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_NPC_Interact::UBTTask_NPC_Interact()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	InteractKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_NPC_Interact, InteractKey));
	Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_NPC_Interact, Target), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_NPC_Interact::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool interacted = OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool(InteractKey.SelectedKeyName);
	if (!interacted) return EBTNodeResult::Failed;


	AAIController* Controller = OwnerComp.GetAIOwner();
	Controller->StopMovement();
	auto TargetActor = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName));
	FVector dir = (TargetActor->GetActorLocation() - Controller->GetPawn()->GetActorLocation()).GetSafeNormal();
	FRotator rot = FRotationMatrix::MakeFromX(dir).Rotator();
	Controller->GetPawn()->SetActorRotation(rot);

	return EBTNodeResult::InProgress;
}

void UBTTask_NPC_Interact::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsBool(InteractKey.SelectedKeyName))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}