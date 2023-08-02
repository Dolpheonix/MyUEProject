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
#include "QuestTable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestButtonEvent, int, index);

UCLASS()
class QUICKSTART_API UQuestButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	FQuestButtonEvent ClickedEvent;

	UFUNCTION()
	void FireClickEvent();

public:
	UPROPERTY(BlueprintReadWrite)
	UButton* QuestSlotButton;
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* QuestSlotNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Index")
	int Index;

	bool Bounded = false;
};

UCLASS()
class QUICKSTART_API UQuestTable : public UUserWidget
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

	// MainQuestList
	UScrollBox* QuestSlotsScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UQuestButton*> QuestSlotButtons;
	// QuestInfo
	UVerticalBox* QuestInfoBox;
	UTextBlock* QuestNameText;
	UTextBlock* QuestInfoText;
	UTextBlock* SubQuestsText;
	
	// Resources
	int SelectedSlotIndex = -1;
};
