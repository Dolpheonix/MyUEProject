#include "DialogueBox.h"
#include "../quickstart.h"
#include "../Core/GameMode/MainGameMode.h"
#include "../Core/GameInstance/MainGameInstance.h"
#include "Blueprint/WidgetTree.h"
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
			if (ResponseButtons[i])
			{
				ResponseButtons[i]->index = i;
				ResponseButtons[i]->DialogueButtonEvent.AddDynamic(this, &UDialogueBox::OnPressed_EndLine);
			}
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
	InteractedNPC = nullptr;

	UUserWidget::NativeDestruct();
}

void UDialogueBox::OpenUI(ANPC* interacted)
{
	check(DialogueText);
	check(ResponseTexts[0] && ResponseTexts[1] && ResponseTexts[2]);
	check(ResponseButtons[0] && ResponseButtons[1] && ResponseButtons[2]);

	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Controller = Cast<APlayerController>(Player->GetController());

	// ��ũ�� ����� ���콺 Ŀ�� ǥ��
	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	InteractedNPC = interacted;

	IsQuestDialogue = false;	// ó������ NPC Dialogue ���
	DialogueLine = InteractedNPC->DialogueTree.GetStartLine();	// ���� Dialogue line�� �ҷ���
	RefreshDialogue();	// �ҷ��� Dialogue line�� ���
}

void UDialogueBox::GetDialogue(int index)
{
	DialogueLine = InteractedNPC->DialogueTree.DialogueLines[index];
	RefreshDialogue();
}

void UDialogueBox::GetQuestDialogue(int index)
{
	// Quest ���൵�� ���� ��ȭ ������ �ٸ�
	switch (TriggeredQuest->Progress)
	{
	case EQuestProgress::Unavailable:
		QuestDialogueLine = TriggeredQuest->DialogueTree_Unavailable.DialogueLines[index];
		break;
	case EQuestProgress::Available:
		QuestDialogueLine = TriggeredQuest->DialogueTree_Available.DialogueLines[index];
		break;
	case EQuestProgress::InProgress:
		QuestDialogueLine = TriggeredQuest->DialogueTree_InProgress.DialogueLines[index];
		break;
	case EQuestProgress::Finished:
		QuestDialogueLine = TriggeredQuest->DialogueTree_Finished.DialogueLines[index];
		break;
	default:
		break;
	}

	RefreshDialogue();
}

void UDialogueBox::OnPressed_EndLine(int index)
{
	if (!IsQuestDialogue)	// NPC Dialogue
	{
		FDialogueResponse Pressed = DialogueLine.Responses[index];	// ���� ��ư�� �´� ����
		for (FDialogueEvent e : Pressed.Events)	// ������ �̺�Ʈ�� ����
		{
			switch (e.EventType)
			{
			case EDialogueEventType::OPENQUEST:
			{
				UMainGameInstance* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (GI)
				{
					QuestIndex = e.QuestIndex;
					TriggeredQuest = GI->GetQuest(QuestIndex);	// QuestIndex�� �´� ����Ʈ �ε�
					if (TriggeredQuest)
					{
						IsQuestDialogue = true;
						QuestDialogueLine = TriggeredQuest->GetStartLine(InteractedNPC->DisplayName);
						RefreshDialogue();
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Couldn't cast GameInstance to UMainGameInstance"));
				}
				break;
			}
			case EDialogueEventType::GIVEITEM:
				Player->Register(InteractedNPC->AcquireItemsInfo[e.ItemIndex]);	// ������ ���
				break;
			case EDialogueEventType::PHASESHIFT:
				InteractedNPC->DialogueTree.StartIndex = e.NextPhaseIndex;	// ���̾�α��� Phase ��ȯ
				break;
			case EDialogueEventType::OPENSHOP:
				InteractedNPC->OpenShop();	// ���� UI�� ��
				break;
			default:
				break;
			}
		}
		if (Pressed.IsEnd) // ��ȭ ���ڸ� �ݾƾ� ��
		{
			Controller->SetShowMouseCursor(false);
			Controller->SetInputMode(FInputModeGameOnly());
			InteractedNPC->UnInteract();
		}
		else if(!IsQuestDialogue) // OPENQUEST �̺�Ʈ�� ���� ��尡 �ٲ��� ���� ���
		{
			GetDialogue(Pressed.NextIndex);
		}
	}
	else
	{
		FQuestDialogueResponse Pressed = QuestDialogueLine.Responses[index];
		for (FQuestDialogueEvent e : Pressed.Events)
		{
			switch (e.EventType)
			{
			case EQuestDialogueEventType::COMMIT:
				Player->RegisterQuest(*TriggeredQuest);	// ����Ʈ�� �÷��̾ ���
				break;
			case EQuestDialogueEventType::COMPLETE:
				Player->EndQuest(*TriggeredQuest);	// ����Ʈ�� �Ϸ� ó��
				break;
			case EQuestDialogueEventType::GIVEITEM:
				Player->Register(InteractedNPC->AcquireItemsInfo[e.ItemIndex]);	// �������� ���
				break;
			case EQuestDialogueEventType::BACKTODIALOGUE:
				IsQuestDialogue = false;
				DialogueLine = InteractedNPC->DialogueTree.DialogueLines[e.BacktoDialogueIndex];	// NPC Dialogue�� ���ƿ�
				RefreshDialogue();
				break;
			case EQuestDialogueEventType::PHASESHIFT:
				InteractedNPC->DialogueTree.StartIndex = e.NextPhaseIndex;	// NPC Dialogue�� Phase�� ��ȯ
				break;
			default:
				break;
			}
		}
		if (Pressed.IsEnd)
		{
			Controller->SetShowMouseCursor(false);
			Controller->SetInputMode(FInputModeGameOnly());
			InteractedNPC->UnInteract();
		}
		else if(IsQuestDialogue)	// Back to Dialogue �̺�Ʈ�� ������� ���� ���
		{
			GetQuestDialogue(Pressed.NextIndex);
		}
	}
}

void UDialogueBox::RefreshDialogue()
{
	if (!IsQuestDialogue)
	{
		FString script = DialogueLine.Speaker + " : " + DialogueLine.TextLine;
		DialogueText->SetText(FText::FromString(script));

		int i;
		for (i = 0; i < DialogueLine.Responses.Num(); i++)
		{
			ResponseTexts[i]->SetText(FText::FromString(DialogueLine.Responses[i].Response));
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
		FString script = QuestDialogueLine.Speaker + " : " + QuestDialogueLine.TextLine;
		DialogueText->SetText(FText::FromString(script));

		int i;
		for (i = 0; i < QuestDialogueLine.Responses.Num(); i++)
		{
			ResponseTexts[i]->SetText(FText::FromString(QuestDialogueLine.Responses[i].Response));
			ResponseButtons[i]->SetIsEnabled(true);
		}
		for (int j = i; j < ResponseTexts.Num(); j++)
		{
			ResponseTexts[j]->SetText(FText::GetEmpty());
			ResponseButtons[j]->SetIsEnabled(false);
		}
	}
}
