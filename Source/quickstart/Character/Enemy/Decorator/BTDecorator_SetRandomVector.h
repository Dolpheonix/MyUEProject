// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_SetRandomVector.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTDecorator_SetRandomVector : public UBTDecorator
{
	GENERATED_BODY()
	
	UBTDecorator_SetRandomVector(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

public:
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector OutputKey;
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector PivotKey;
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	float Radius;
};
