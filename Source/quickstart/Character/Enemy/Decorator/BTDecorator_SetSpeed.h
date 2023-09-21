// Decorator_SetSpeed
// NPC의 이동 속도를 조정하는 데코레이터
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
	// 이동 속도
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed = 200.0f;
};
