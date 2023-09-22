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

	// 스크린 가운데에 마우스 커서 표시
	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	InteractedNPC = interacted;

	IsQuestDialogue = false;	// 처음에는 NPC Dialogue 모드
	DialogueLine = InteractedNPC->DialogueTree.GetStartLine();	// 시작 Dialogue line을 불러옴
	RefreshDialogue();	// 불러온 Dialogue line을 출력
}

void UDialogueBox::GetDialogue(int index)
{
	DialogueLine = InteractedNPC->DialogueTree.DialogueLines[index];
	RefreshDialogue();
}

void UDialogueBox::GetQuestDialogue(int index)
{
	// Quest 진행도에 따라 대화 내용이 다름
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
		FDialogueResponse Pressed = DialogueLine.Responses[index];	// 눌린 버튼에 맞는 응답
		for (FDialogueEvent e : Pressed.Events)	// 응답의 이벤트를 적용
		{
			switch (e.EventType)
			{
			case EDialogueEventType::OPENQUEST:
			{
				UMainGameInstance* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (GI)
				{
					QuestIndex = e.QuestIndex;
					TriggeredQuest = GI->GetQuest(QuestIndex);	// QuestIndex에 맞는 퀘스트 로드
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
				Player->Register(InteractedNPC->AcquireItemsInfo[e.ItemIndex]);	// 아이템 등록
				break;
			case EDialogueEventType::PHASESHIFT:
				InteractedNPC->DialogueTree.StartIndex = e.NextPhaseIndex;	// 다이얼로그의 Phase 전환
				break;
			case EDialogueEventType::OPENSHOP:
				InteractedNPC->OpenShop();	// 상점 UI를 엶
				break;
			default:
				break;
			}
		}
		if (Pressed.IsEnd) // 대화 상자를 닫아야 함
		{
			Controller->SetShowMouseCursor(false);
			Controller->SetInputMode(FInputModeGameOnly());
			InteractedNPC->UnInteract();
		}
		else if(!IsQuestDialogue) // OPENQUEST 이벤트에 의해 모드가 바뀌지 않은 경우
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
				Player->RegisterQuest(*TriggeredQuest);	// 퀘스트를 플레이어에 등록
				break;
			case EQuestDialogueEventType::COMPLETE:
				Player->EndQuest(*TriggeredQuest);	// 퀘스트를 완료 처리
				break;
			case EQuestDialogueEventType::GIVEITEM:
				Player->Register(InteractedNPC->AcquireItemsInfo[e.ItemIndex]);	// 아이템을 등록
				break;
			case EQuestDialogueEventType::BACKTODIALOGUE:
				IsQuestDialogue = false;
				DialogueLine = InteractedNPC->DialogueTree.DialogueLines[e.BacktoDialogueIndex];	// NPC Dialogue로 돌아옴
				RefreshDialogue();
				break;
			case EQuestDialogueEventType::PHASESHIFT:
				InteractedNPC->DialogueTree.StartIndex = e.NextPhaseIndex;	// NPC Dialogue의 Phase를 전환
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
		else if(IsQuestDialogue)	// Back to Dialogue 이벤트가 적용되지 않은 경우
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
