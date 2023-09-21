// Behavior Tree Task - NPC Idle
// NPC�� Wait �½�ũ
// �÷��̾ ��ȣ�ۿ��� �� ���� ó��
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_NPC_Idle.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTTask_NPC_Idle : public UBTTask_Wait
{
	GENERATED_BODY()
	
	UBTTask_NPC_Idle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// ��ȣ�ۿ� ���θ� Ȯ���ϴ� ������ Ű
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector InteractKey;
};
