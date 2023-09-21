// UBTTask_Bump
// ABighead�� ���� �½�ũ

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../Bighead.h"
#include "../../../Main/MainCharacter.h"
#include "BTTask_Bump.generated.h"

UCLASS()
class QUICKSTART_API UBTTask_Bump : public UBTTask_BlackboardBase	// UBTTask_BlackboardBase�� Blackboard Key : ���� Ÿ��
{
	GENERATED_BODY()
	
	UBTTask_Bump();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	// ���� Enemy�� Detection Mode
	UPROPERTY(EditAnywhere, Category="BlackboardKey")
	FBlackboardKeySelector DetectionModeKey;
	// ���� ����
	UPROPERTY(EditAnywhere, Category="Range")
	float AttackRange = 30.0f;
	// AI ��Ʈ�ѷ�
	AAIController* Controller;
	// �½�ũ�� �������� ����
	ABighead* OwnerBighead;
	// ���� Ÿ�� (���� ĳ����)
	AMainCharacter* Target;
	// Ÿ�ٿ� �� ����� ����
	FVector Impulse;
};
