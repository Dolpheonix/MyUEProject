// Behavior Tree Task - Chase
// Ÿ���� �i�ư��� MoveTo �½�ũ�� ���
// ���� ��忡 ���� �½�ũ �ߴ� ���θ� �����ϵ��� ����� �߰�
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_Chase.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_Chase : public UBTTask_MoveTo
{
	GENERATED_BODY()

	UBTTask_Chase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// ���� ���� ���
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
};
