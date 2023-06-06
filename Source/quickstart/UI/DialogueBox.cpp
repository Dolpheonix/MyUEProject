// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueBox.h"
#include "../quickstartGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UDialogueBox::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
	bIsFocusable = true;
	if (!Bounded)
	{
		DialogueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DialogueText")));

		ResponseTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ResponseT0"))));
		ResponseTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ResponseT1"))));
		ResponseTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ResponseT2"))));

		ResponseButtons.Add(Cast<UDialogueButton>(GetWidgetFromName(TEXT("ResponseB0"))));
		ResponseButtons.Add(Cast<UDialogueButton>(GetWidgetFromName(TEXT("ResponseB1"))));
		ResponseButtons.Add(Cast<UDialogueButton>(GetWidgetFromName(TEXT("ResponseB2"))));

		for (int i = 0; i < 3; i++)
		{
			ResponseButtons[i]->index = i;
			ResponseButtons[i]->DialogueButtonEvent.AddDynamic(this, &UDialogueBox::GetNextDialogue);
		}
		Bounded = true;
	}
}

void UDialogueBox::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void UDialogueBox::NativeDestruct()
{
	DialogueTree.Empty();
	InteractedNPC = nullptr;

	UUserWidget::NativeDestruct();
}

void UDialogueBox::InitDialogue(TArray<FDialogueLine> dialogues, ANPC* Interacted)
{
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Controller = Cast<APlayerController>(Player->GetController());

	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	InteractedNPC = Interacted;
	DialogueTree = dialogues;
	currIndex = 0;

	RefreshDialogue();
}

void UDialogueBox::InitQuestDialogue(int index)
{
	TriggeredQuest = &InteractedNPC->Quests[index];
	QuestIndex = index;
	switch (TriggeredQuest->Progress)
	{
	case EQuestProgress::Available:
		DialogueTree = TriggeredQuest->CommitDialogue;
		break;
	case EQuestProgress::InProgress:
		DialogueTree = TriggeredQuest->InProgressDialogue;
		break;
	case EQuestProgress::Finished:
		DialogueTree = TriggeredQuest->FinishDialogue;
		break;
	default:
		break;
	}
	currIndex = 0;
	RefreshDialogue();
}

void UDialogueBox::GetNextDialogue(int index)
{
	if (DialogueTree[currIndex].Responses[index].isEnd)
	{
		EndDialogue(currIndex, index);
	}
	else
	{
		currIndex = DialogueTree[currIndex].Responses[index].nextIndex;

		RefreshDialogue();
	}
}

void UDialogueBox::EndDialogue(int index, int responseIndex)
{
	// TODO //
	switch (DialogueTree[index].Responses[responseIndex].EndContext)
	{
	case EDialogueEndType::DEFAULT:
		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
		InteractedNPC->UnInteract();
		break;
	case EDialogueEndType::SHOP:
		InteractedNPC->OpenShop();
		break;
	case EDialogueEndType::QUEST_START:
		InteractedNPC->OpenQuestDialogue(DialogueTree[index].Responses[responseIndex].QuestIndex);
		break;
	case EDialogueEndType::QUEST_COMMIT:
		InteractedNPC->GiveQuest(QuestIndex);
		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
		InteractedNPC->UnInteract();
		break;
	case EDialogueEndType::QUEST_END:
		InteractedNPC->EndQuest(QuestIndex);
		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
		InteractedNPC->UnInteract();
		break;
	default:
		break;
	}
}

void UDialogueBox::RefreshDialogue()
{
	if (currIndex < DialogueTree.Num())
	{
		FString script = DialogueTree[currIndex].Speaker + " : " + DialogueTree[currIndex].TextLine;
		DialogueText->SetText(FText::FromString(script));

		int i;
		for (i = 0; i < DialogueTree[currIndex].Responses.Num(); i++)
		{
			ResponseTexts[i]->SetText(FText::FromString(DialogueTree[currIndex].Responses[i].Response));
			ResponseButtons[i]->SetIsEnabled(true);
		}
		for (int j = i; j < ResponseTexts.Num(); j++)
		{
			ResponseTexts[j]->SetText(FText::GetEmpty());
			ResponseButtons[j]->SetIsEnabled(false);
		}
	}
	else
	{
		InteractedNPC->UnInteract();
	}
}
