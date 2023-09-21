// UBTDecorator_SetRandomVector
// Enemy 공통 데코레이터
// 범위 내 랜덤 지점을 리턴

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_SetRandomVector.generated.h"

UCLASS()
class QUICKSTART_API UBTDecorator_SetRandomVector : public UBTDecorator
{
	GENERATED_BODY()
	
	UBTDecorator_SetRandomVector(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

public:
	// 랜덤 지점을 출력할 키
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector OutputKey;
	// 기준점
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector PivotKey;
	// 범위
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	float Radius;
};
