// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "../Character/NPC/NPC.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "DialogueButton.h"
#include "DialogueBox.generated.h"
/**
 * 
 */
UCLASS()
class QUICKSTART_API UDialogueBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void OpenUI(ANPC* interacted);
	void GetDialogue(int index);
	void GetQuestDialogue(int index);
	void RefreshDialogue();

	UFUNCTION()
	void OnPressed_EndLine(int index);

	void EndDialogue(int index, int responseIndex);

	AMainCharacter* Player;
	APlayerController* Controller;
	ANPC* InteractedNPC;

	FQuest* TriggeredQuest;
	int QuestIndex;

	FDialogueLine DialogueLine;
	FQuestDialogueLine QuestDialogueLine;

	bool IsQuestDialogue;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* DialogueText;
	UPROPERTY(BlueprintReadWrite)
	UButton* NextButton;
	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> ResponseTexts;
	UPROPERTY(BlueprintReadWrite)
	TArray<UDialogueButton*> ResponseButtons;

	bool Bounded = false;
};
