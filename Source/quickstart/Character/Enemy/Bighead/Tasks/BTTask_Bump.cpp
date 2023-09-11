// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Bump.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../BigheadController.h"

UBTTask_Bump::UBTTask_Bump()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;

	DetectionModeKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Bump, DetectionModeKey), FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyDetectionMode")));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Bump, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_Bump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	elapsed = 0.0f;

	Controller = OwnerComp.GetAIOwner();
	auto blackboard = Controller->GetBlackboardComponent();

	EEnemyDetectionMode Mode = EEnemyDetectionMode(blackboard->GetValueAsEnum(DetectionModeKey.SelectedKeyName));
	if (Mode != EEnemyDetectionMode::DETECTED) return EBTNodeResult::Failed;

	Target = Cast<AMainCharacter>(blackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
	if (Target)
	{
		OwnerBighead = Cast<ABighead>(Controller->GetCharacter());
		if (FVector::Dist2D(Target->GetActorLocation(), OwnerBighead->GetActorLocation()) <= AttackRange)
		{
			Impulse = (Target->GetActorLocation() - OwnerBighead->GetActorLocation()).GetSafeNormal2D() * 0.2f;
			Impulse.Z += 0.03f;
			OwnerBighead->BumpAuraComponent->Activate(true);
			OwnerBighead->Bump(Target);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_Bump::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	elapsed += DeltaSeconds;
	Target->GetCharacterMovement()->AddInputVector(Impulse);
	if (!Target->bHurt)
	{
		OwnerBighead->BumpAuraComponent->Deactivate();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}