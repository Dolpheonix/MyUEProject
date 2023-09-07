// Fill out your copyright notice in the Description page of Project Settings.


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
	if (Mode != EEnemyDetectionMode::DETECTED) return EBTNodeResult::Failed;

	FVector dir = (TargetActor->GetActorLocation() - OwnerSniper->GetActorLocation()).GetSafeNormal();
	FRotator rot = FRotationMatrix::MakeFromX(dir).Rotator();

	OwnerSniper->SetActorRotation(rot);

	OwnerSniper->Fire();

	return EBTNodeResult::InProgress;
}

void UBTTask_Fire::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto isEnded = !OwnerSniper->GetMesh()->GetSingleNodeInstance()->IsPlaying();
	EEnemyDetectionMode Mode = EEnemyDetectionMode(Controller->GetBlackboardComponent()->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (isEnded || Mode != EEnemyDetectionMode::DETECTED)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
