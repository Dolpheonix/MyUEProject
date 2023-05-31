// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "QuestStatus.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FQuestTextBlock
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* MainBlock;
	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> SubBlocks;
};

UCLASS()
class QUICKSTART_API UQuestStatus : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void InitQuestUI(AActor* Instigator);

public:
	AMainCharacter* Player;
	APlayerController* Controller;
	bool Bounded = false;

	TArray<FQuestTextBlock> QuestBlocks;
	UCanvasPanel* RootCanvas;
};
