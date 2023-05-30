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
	//virtual void NativeTick(const FGeometry& Mygeometry, float InDeltaTime) override;
	//virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void InitDialogue(TArray<FDialogueLine> dialogues, ANPC* Interacted);
	void InitQuestDialogue(int index);
	void RefreshDialogue();

	UFUNCTION()
	void GetNextDialogue(int index);

	void EndDialogue(int index);

	AMainCharacter* Player;

	APlayerController* Controller;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* DialogueText;

	UPROPERTY(BlueprintReadWrite)
	ANPC* InteractedNPC;

	FQuest* TriggeredQuest;

	UPROPERTY(BlueprintReadWrite)
	int QuestIndex;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueLine> DialogueTree;

	UPROPERTY(BlueprintReadWrite)
	UButton* NextButton;

	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> ResponseTexts;

	UPROPERTY(BlueprintReadWrite)
	TArray<UDialogueButton*> ResponseButtons;

	int currIndex = 0;

	bool Bounded = false;
};
