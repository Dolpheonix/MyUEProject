// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/Overlay.h"
#include "QuestStatus.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestButtonEvent, int, index);

UCLASS()
class QUICKSTART_API UQuestButton : public UCanvasPanel
{
	GENERATED_BODY()

public:
	UQuestButton();

	void SetQuest(FQuest* quest);
	void InitText(UDataTable* font);
	void SetQsText(FString str);
	UFUNCTION()
	void OnPressed_Broadcast();
	FString GetDisplayName(FString name);

	FQuestButtonEvent PressedEvent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UButton* SlotButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URichTextBlock* SlotText;

	int QuestIndex = -1;
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
	void RefreshBlocks();
	UFUNCTION()
	void OpenQuestInfo(int index);

	FString GetSubQuestText(FSingleQuest* subquest);

public:
	AMainCharacter* Player;
	APlayerController* Controller;
	bool Bounded = false;

	// Widgets
	UCanvasPanel* RootCanvas;
	// MainQuestList
	UScrollBox* MainQuestScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UQuestButton*> MainQuestBlocks;
	// QuestInfo
	UVerticalBox* QuestInfoBox;
	URichTextBlock* QuestNameText;
	UScrollBox* QuestInfoScroll;
	URichTextBlock* QuestInfoText;
	UScrollBox* SubQuestsScroll;
	URichTextBlock* SubQuestsText;
	
	// Resources
	UDataTable* MainFont;
	FSlateBrush QuestSlotBrush_N;
	FSlateBrush QuestSlotBrush_H;
	FSlateBrush QuestSlotBrush_S;

	int SelectedSlotIndex = -1;
};
