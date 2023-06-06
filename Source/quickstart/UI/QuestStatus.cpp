// Fill out your copyright notice in the Description page of Project Settings.
#define MAXCHAR_PER_LINE 25

#include "QuestStatus.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/Actor.h"

void UQuestStatus::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
}

void UQuestStatus::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void UQuestStatus::NativeDestruct()
{
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());
	UUserWidget::NativeDestruct();
}

FReply UQuestStatus::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	int ascii = InKeyEvent.GetCharacter();
	if (InKeyEvent.GetKey() == EKeys::Q)
	{
		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
		Player->GameMode->ChangeMenuWidget(Player->GameMode->MainUI);
	}

	return FReply::Handled();
}

void UQuestStatus::InitQuestUI(AActor* Instigator)
{
	bIsFocusable = true;
	if (!Bounded)
	{
		Player = Cast<AMainCharacter>(Instigator);
		Controller = Cast<APlayerController>(Player->GetController());

		RichTextSet = Helpers::LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Font/DT_FontSet.DT_FontSet"));
		RootCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("RootCanvas")));
		Bounded = true;
	}

	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());


	int numslot = 0;

	for (int i = 0; i < Player->WorkingQuests.Num(); i++)
	{
		if (i >= QuestBlocks.Num())
		{
			QuestBlocks.Add(FQuestTextBlock());
			QuestBlocks[i].MainBlock = WidgetTree->ConstructWidget<URichTextBlock>(URichTextBlock::StaticClass(), FName("Quest " + i));
			QuestBlocks[i].MainBlock->SetTextStyleSet(RichTextSet);
			RootCanvas->AddChild(QuestBlocks[i].MainBlock);
		}
		QuestBlocks[i].MainBlock->SetRenderTranslation(FVector2D(100.0f, numslot * 80.0f + 200.0f));
		FString mainstr = GetMainQuestText(Player->WorkingQuests[i]->Name, MAXCHAR_PER_LINE, Player->WorkingQuests[i]->Progress == EQuestProgress::Finished);
		QuestBlocks[i].MainBlock->SetText(FText::FromString(mainstr));

		numslot++;
		for (int j = 0; j < Player->WorkingQuests[i]->SubQuests.Num(); j++)
		{
			if (j >= QuestBlocks[i].SubBlocks.Num())
			{
				FString subname = "SubBlock" + i;
				subname += "-" + j;
				QuestBlocks[i].SubBlocks.Add(WidgetTree->ConstructWidget<URichTextBlock>(URichTextBlock::StaticClass(), FName(subname)));
				QuestBlocks[i].SubBlocks[j]->SetTextStyleSet(RichTextSet);
				RootCanvas->AddChild(QuestBlocks[i].SubBlocks[j]);
			}
			QuestBlocks[i].SubBlocks[j]->SetRenderTranslation(FVector2D(150.0f, numslot * 60.0f + 200.0f));

			if (Player->WorkingQuests[i]->SubQuests[j].Completed)
			{
				FString str = "<Strike>" + Player->WorkingQuests[i]->SubQuests[j].Name;
				str += "</>";
				QuestBlocks[i].SubBlocks[j]->SetText(FText::FromString(str));
			}
			else QuestBlocks[i].SubBlocks[j]->SetText(FText::FromString(Player->WorkingQuests[i]->SubQuests[j].Name));
			numslot++;
		}

		for (int j = Player->WorkingQuests[i]->SubQuests.Num(); j < QuestBlocks[i].SubBlocks.Num(); j++)
		{
			QuestBlocks[i].SubBlocks[j]->SetText(FText::GetEmpty());
		}
	}

	for (int i = Player->WorkingQuests.Num(); i < QuestBlocks.Num(); i++)
	{
		QuestBlocks[i].MainBlock->SetText(FText::GetEmpty());
		for (int j = 0; j < QuestBlocks[i].SubBlocks.Num(); j++)
		{
			QuestBlocks[i].SubBlocks[j]->SetText(FText::GetEmpty());
		}
	}
}

FString UQuestStatus::GetMainQuestText(FString origin, int maxperline, bool isStriked)
{
	int cnt = origin.Len() / (maxperline + 1);

	for(int i = 0; i < cnt; i++)
	{
		origin.InsertAt((i+1) * maxperline + i, "\n");
	}

	return origin;
}
FString UQuestStatus::GetSubQuestText(FString origin, int maxperline, bool isStriked)
{
	return origin;
}