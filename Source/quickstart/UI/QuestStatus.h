// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/RichTextBlock.h"
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
	URichTextBlock* MainBlock;
	UPROPERTY(BlueprintReadWrite)
	TArray<URichTextBlock*> SubBlocks;
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

	FString GetMainQuestText(FString origin, int maxperline, bool isStriked);
	FString GetSubQuestText(FString origin, int maxperline, bool isStriked);

public:
	AMainCharacter* Player;
	APlayerController* Controller;
	bool Bounded = false;

	TArray<FQuestTextBlock> QuestBlocks;
	UCanvasPanel* RootCanvas;
	
	UDataTable* RichTextSet;
};
