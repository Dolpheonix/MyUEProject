// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_SetRandomVector.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTDecorator_SetRandomVector::UBTDecorator_SetRandomVector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyActivation = true;

	OutputKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_SetRandomVector, OutputKey));
	PivotKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_SetRandomVector, PivotKey));
}

void UBTDecorator_SetRandomVector::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	AAIController* Controller = SearchData.OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
	FVector Pivot = Blackboard->GetValueAsVector(PivotKey.SelectedKeyName);
	FVector LocationNow = Controller->GetCharacter()->GetActorLocation();
	while (true)
	{
		FVector Destination = UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), Pivot, Radius);
		if (FVector::Distance(LocationNow, Destination) >= Radius * 0.5)
		{
			Blackboard->SetValueAsVector(OutputKey.SelectedKeyName, Destination);
			break;
		}
	}
}