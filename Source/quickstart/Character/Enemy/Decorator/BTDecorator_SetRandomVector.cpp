#include "BTDecorator_SetRandomVector.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTDecorator_SetRandomVector::UBTDecorator_SetRandomVector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyActivation = true;

	OutputKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_SetRandomVector, OutputKey)); // 벡터만 키로 지정 가능
	PivotKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_SetRandomVector, PivotKey));
}

void UBTDecorator_SetRandomVector::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	AAIController* Controller = SearchData.OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
	FVector Pivot = Blackboard->GetValueAsVector(PivotKey.SelectedKeyName);	// Pivot : 액터의 시작 위치
	FVector LocationNow = Controller->GetCharacter()->GetActorLocation();	// LocationNow : 액터의 현재 위치
	while (true)
	{
		FVector Destination = UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), Pivot, Radius); // Pivot 기준으로 Radius 내의 랜덤 지점 리턴
		if (FVector::Distance(LocationNow, Destination) >= Radius * 0.5) // 현재 위치에서 일정 거리만큼은 떨어져 있어야 설정 가능
		{
			Blackboard->SetValueAsVector(OutputKey.SelectedKeyName, Destination);
			break;
		}
	}
}