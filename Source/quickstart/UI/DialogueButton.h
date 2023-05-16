// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "DialogueButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueButtonEvent, int, index);
/**
 * 
 */
UCLASS()
class QUICKSTART_API UDialogueButton : public UButton
{
	GENERATED_BODY()
	
public:
	UDialogueButton();
	UDialogueButton(int i);

	UFUNCTION()
	void ClickTrigger();

	FDialogueButtonEvent DialogueButtonEvent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	int index;
};
