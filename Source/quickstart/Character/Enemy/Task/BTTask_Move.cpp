// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Move.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "../../../Utils/AIUtil.h"

UBTTask_Move::UBTTask_Move()
{
	bNotifyTick = true;
	bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTTask_Move::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	Controller = OwnerComp.GetAIOwner();
	OwnerActor = Controller->GetCharacter();

	Blackboard = Controller->GetBlackboardComponent();
	auto BlackboardData = GetBlackboardAsset();
	Target.ResolveSelectedKey(*BlackboardData);

	if (Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(Target.SelectedKeyName));
		bFollowActor = true;
	}
	else if (Target.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		TargetVector = Blackboard->GetValueAsVector(Target.SelectedKeyName);
		bFollowActor = false;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_Move::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector TargetLoc = bFollowActor ? TargetActor->GetActorLocation() : TargetVector;
	FVector OwnerLoc = OwnerActor->GetActorLocation();
	FVector Dist = TargetLoc - OwnerLoc;
	if (Dist.Size() <= Radius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (bFollowActor)
	{
		if (AIHelper::isValidPath(GetWorld(), OwnerLoc, TargetLoc))
		{
			Controller->K2_SetFocus(TargetActor);
			Dist.Normalize();

			FHitResult hit;
			OwnerActor->AddActorWorldOffset(Dist * 5.0f, true, &hit);

			if (hit.bBlockingHit) OwnerActor->Jump();
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
	else
	{
		Controller->SetFocalPoint(TargetLoc);
		Dist.Normalize();
		FHitResult hit;
		OwnerActor->AddActorWorldOffset(Dist * 5.0f, true, &hit);
		if (hit.bBlockingHit) OwnerActor->Jump();
	}
}


