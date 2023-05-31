// Fill out your copyright notice in the Description page of Project Settings.


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
			QuestBlocks[i].MainBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName("Quest " + i));
			RootCanvas->AddChild(QuestBlocks[i].MainBlock);
		}
		QuestBlocks[i].MainBlock->SetRenderTranslation(FVector2D(100.0f, numslot * 80.0f + 200.0f));
		QuestBlocks[i].MainBlock->SetText(FText::FromString(Player->WorkingQuests[i]->Name));
		numslot++;
		for (int j = 0; j < Player->WorkingQuests[i]->SubQuests.Num(); j++)
		{
			if (j >= QuestBlocks[i].SubBlocks.Num())
			{
				FString subname = "SubBlock" + i;
				subname += "-" + j;
				QuestBlocks[i].SubBlocks.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(subname)));
				RootCanvas->AddChild(QuestBlocks[i].SubBlocks[j]);
			}
			QuestBlocks[i].SubBlocks[j]->SetRenderTranslation(FVector2D(150.0f, numslot * 80.0f + 200.0f));
			QuestBlocks[i].SubBlocks[j]->SetText(FText::FromString(Player->WorkingQuests[i]->SubQuests[j].Name));
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