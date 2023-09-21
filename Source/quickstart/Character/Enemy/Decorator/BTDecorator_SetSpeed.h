// Decorator_SetSpeed
// NPC�� �̵� �ӵ��� �����ϴ� ���ڷ�����
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_SetSpeed.generated.h"

UCLASS()
class QUICKSTART_API UBTDecorator_SetSpeed : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_SetSpeed();

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

public:
	// �̵� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed = 200.0f;
};
