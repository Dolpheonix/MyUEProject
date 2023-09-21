// Behavior Tree Task - NPC Interact
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NPC_Interact.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_NPC_Interact : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_NPC_Interact();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// ��ȣ�ۿ� ���θ� Ȯ���ϴ� ������ Ű
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector InteractKey;
	// ��ȣ�ۿ� ���� Ÿ��(�÷��̾�)
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector Target;
};
