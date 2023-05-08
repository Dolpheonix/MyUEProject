// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_SetSpeed.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UBTDecorator_SetSpeed : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_SetSpeed();

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

public:
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed = 200.0f;
};
