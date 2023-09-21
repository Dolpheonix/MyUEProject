#include "BTDecorator_SetRandomVector.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTDecorator_SetRandomVector::UBTDecorator_SetRandomVector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyActivation = true;

	OutputKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_SetRandomVector, OutputKey)); // ���͸� Ű�� ���� ����
	PivotKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_SetRandomVector, PivotKey));
}

void UBTDecorator_SetRandomVector::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	AAIController* Controller = SearchData.OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
	FVector Pivot = Blackboard->GetValueAsVector(PivotKey.SelectedKeyName);	// Pivot : ������ ���� ��ġ
	FVector LocationNow = Controller->GetCharacter()->GetActorLocation();	// LocationNow : ������ ���� ��ġ
	while (true)
	{
		FVector Destination = UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), Pivot, Radius); // Pivot �������� Radius ���� ���� ���� ����
		if (FVector::Distance(LocationNow, Destination) >= Radius * 0.5) // ���� ��ġ���� ���� �Ÿ���ŭ�� ������ �־�� ���� ����
		{
			Blackboard->SetValueAsVector(OutputKey.SelectedKeyName, Destination);
			break;
		}
	}
}