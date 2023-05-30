// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/TextBlock.h"
#include "QuestStatus.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UQuestStatus : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void InitQuestUI(AActor* Instigator);

public:
	AMainCharacter* Player;
	APlayerController* Controller;
	bool Bounded = false;

	TArray<UTextBlock*> QuestTexts;
};
