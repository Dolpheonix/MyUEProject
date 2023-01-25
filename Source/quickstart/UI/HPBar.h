// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HPBar.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UHPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite)
	UProgressBar* HP_ProgressBar;

};
