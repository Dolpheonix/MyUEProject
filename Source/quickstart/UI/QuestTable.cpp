// Fill out your copyright notice in the Description page of Project Settings.
#define MAXCHAR_PER_LINE 25
#define MAXQUEST 20

#include "QuestTable.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/Actor.h"

void UQuestButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!Bounded)
	{
		QuestSlotButton = Cast<UButton>(GetWidgetFromName(TEXT("QuestSlot")));
		if (QuestSlotButton) QuestSlotButton->OnClicked.AddDynamic(this, &UQuestButton::FireClickEvent);
		QuestSlotNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestSlotName")));

		Bounded = true;
	}
}

void UQuestButton::NativeConstruct()
{
	Super::NativeConstruct();

	check(QuestSlotButton);
	check(QuestSlotNameText);
}

void UQuestButton::FireClickEvent()
{
	ClickedEvent.Broadcast(Index);
}

void UQuestTable::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();

	if (!Bounded)
	{
		QuestSlotsScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("QuestSlots")));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_0"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_1"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_2"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_3"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_4"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_5"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_6"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_7"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_8"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_9"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_10"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_11"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_12"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_13"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_14"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_15"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_16"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_17"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_18"))));
		QuestSlotButtons.Add(Cast<UQuestButton>(GetWidgetFromName(TEXT("QuestSlot_19"))));

		for (int i = 0; i < MAXQUEST; i++)
		{
			if (QuestSlotButtons[i])
			{
				QuestSlotButtons[i]->ClickedEvent.AddDynamic(this, &UQuestTable::OpenQuestInfo);
				QuestSlotButtons[i]->Index = i;
			}
		}

		QuestInfoBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("QuestBox")));
		QuestNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestName")));
		QuestInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestInfo")));
		SubQuestsText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SubQuests")));

		if (QuestNameText && QuestInfoText && SubQuestsText)
		{
			QuestNameText->SetText(FText::FromString("Quest Name"));
			QuestInfoText->SetText(FText::FromString("Quest Info. This slot is for quest's information. And the reason I wroting this loooooong letter is for testing auto line breaking. Thanks!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
			SubQuestsText->SetText(FText::FromString("SubQuest Text"));
		}		
		Bounded = true;
	}
}

void UQuestTable::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	check(QuestSlotsScroll);
	for (int i = 0; i < 20; i++)
	{
		check(QuestSlotButtons[i]);
	}
	check(QuestInfoBox);
	check(QuestNameText);
	check(QuestInfoText);
	check(SubQuestsText);
}

void UQuestTable::NativeDestruct()
{
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());
	UUserWidget::NativeDestruct();
}

FReply UQuestTable::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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

void UQuestTable::InitQuestUI(AActor* Instigator)
{
	bIsFocusable = true;

	Player = Cast<AMainCharacter>(Instigator);
	Controller = Cast<APlayerController>(Player->GetController());

	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	RefreshBlocks();
}

void UQuestTable::RefreshBlocks()
{
	for (int i = 0; i < Player->QuestList.WorkingQuests.Num(); i++)
	{
		QuestSlotButtons[i]->SetVisibility(ESlateVisibility::Visible);
		QuestSlotButtons[i]->QuestSlotNameText->SetText(FText::FromString(Player->QuestList.WorkingQuests[i]->Name));
	}
	for (int i = Player->QuestList.WorkingQuests.Num(); i < MAXQUEST; i++)
	{
		QuestSlotButtons[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	QuestNameText->SetText(FText::GetEmpty());
	QuestInfoText->SetText(FText::GetEmpty());
	SubQuestsText->SetText(FText::GetEmpty());
}

FString UQuestTable::GetSubQuestText(FSingleQuest* subquest)
{
	FString str;
	switch (subquest->Type)
	{
	case ESingleQuestType::Arrival:
		str += TEXT("빛나는 곳으로 이동");
		if (subquest->Completed) str += TEXT("	(완료)");
		break;
	case ESingleQuestType::Hunt:
	{
		str += TEXT("해당 적을 처치");
		if (subquest->Completed) str += TEXT("	(완료)");
		str += "\n";
		for (int i = 0; i < subquest->HuntingLists.Num(); i++)
		{
			FString oneline;
			oneline += "\t";
			oneline += subquest->HuntingLists[i].Huntee;
			oneline += "   ";
			oneline += FString::FromInt(subquest->currAmounts[i]);
			oneline += "/" + FString::FromInt(subquest->HuntingLists[i].HuntAmount);

			str += oneline;
			str += "\n";
		}
		break;
	}
	case ESingleQuestType::Item:
	{
		str += TEXT("해당 아이템을 수집");
		if (subquest->Completed) str += TEXT("	(완료)");
		str += "\n";
		for (int i = 0; i < subquest->ItemLists.Num(); i++)
		{
			FString oneline;
			oneline += "\t";
			oneline += subquest->ItemLists[i].ItemName;
			oneline += "   ";
			oneline += FString::FromInt(subquest->currAmounts[i]);
			oneline += "/" + FString::FromInt(subquest->ItemLists[i].ItemAmount);
			
			str += oneline;
			str += "\n";
		}
		break;
	}
	case ESingleQuestType::Action:
		str += subquest->ActionInfo;
		if (subquest->Completed) str += TEXT("	(완료)");
		break;
	default:
		break;
	}
	return str;
}

void UQuestTable::OpenQuestInfo(int index)
{
	// Slot Selection
	if (SelectedSlotIndex >= 0)
	{
		FSlateBrush NormalBrush = QuestSlotButtons[SelectedSlotIndex]->QuestSlotButton->WidgetStyle.Normal;
		NormalBrush.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
		QuestSlotButtons[SelectedSlotIndex]->QuestSlotButton->WidgetStyle.SetNormal(NormalBrush);
	}
	SelectedSlotIndex = index;
	FSlateBrush SelectedBrush = QuestSlotButtons[SelectedSlotIndex]->QuestSlotButton->WidgetStyle.Normal;
	SelectedBrush.TintColor = FSlateColor(FLinearColor(0.0f, 0.0f, 1.0f));
	QuestSlotButtons[SelectedSlotIndex]->QuestSlotButton->WidgetStyle.SetNormal(SelectedBrush);
	
	FQuest* Selected = Player->QuestList.WorkingQuests[index];

	QuestNameText->SetText(FText::FromString("\n" + Selected->Name));
	QuestInfoText->SetText(FText::FromString(Selected->Info));

	FString subquestString;
	if (Selected->Type == EQuestType::Serial)
	{
		for (int i = 0; i <= Selected->CurrPhase; i++)
		{
			if (i >= Selected->SubQuests.Num()) break;
			FString str = GetSubQuestText(&Selected->SubQuests[i]);
			subquestString += str;
			subquestString += "\n";
		}
	}
	else
	{
		for (int i = 0; i < Player->QuestList.WorkingQuests[index]->SubQuests.Num(); i++)
		{
			FString str = GetSubQuestText(&Player->QuestList.WorkingQuests[index]->SubQuests[i]);
			subquestString += str;
			subquestString += "\n";
		}
	}

	SubQuestsText->SetText(FText::FromString(subquestString));
}