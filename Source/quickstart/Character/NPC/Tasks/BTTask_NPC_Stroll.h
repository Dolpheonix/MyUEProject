// Behavior Tree Task - NPC Stroll
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_NPC_Stroll.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_NPC_Stroll : public UBTTask_MoveTo
{
	GENERATED_BODY()

	UBTTask_NPC_Stroll();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// ��ȣ�ۿ� ���θ� Ȯ���ϴ� ������ Ű
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector InteractKey;
};
