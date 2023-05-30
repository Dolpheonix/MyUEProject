// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestStatus.h"
#include "GameFramework/Actor.h"

void UQuestStatus::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
	bIsFocusable = true;
	if (!Bounded)
	{
		QuestTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_QuestName0"))));
		QuestTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_QuestName1"))));
		QuestTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_QuestName2"))));
		QuestTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_QuestName3"))));
		QuestTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_QuestName4"))));

		Bounded = true;
	}
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

void UQuestStatus::InitQuestUI(AActor* Instigator)
{
	Player = Cast<AMainCharacter>(Instigator);
	Controller = Cast<APlayerController>(Player->GetController());

	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	for (int i = 0; i < Player->WorkingQuests.Num(); i++)
	{
		QuestTexts[i]->SetText(FText::FromString(Player->WorkingQuests[i]->Name));
	}
}