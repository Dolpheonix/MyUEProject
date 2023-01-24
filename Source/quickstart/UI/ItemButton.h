// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ItemButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemButtonEvent, int, index, ETypeTag, type);

/**
 * 
 */
UCLASS()
class QUICKSTART_API UItemButton : public UButton
{
	GENERATED_BODY()
	
public:
	UItemButton();
	UItemButton(int i);

	void SetEvent();

	UFUNCTION()
	void HoverTrigger();
	UFUNCTION()
	void ClickTrigger();
	UFUNCTION()
	void PressTrigger();

	FItemButtonEvent HoveredEvent;
	FItemButtonEvent ClickedEvent;
	FItemButtonEvent PressedEvent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button")
	int index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	ETypeTag type;
};
