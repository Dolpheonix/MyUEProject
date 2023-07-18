// Fill out your copyright notice in the Description page of Project Settings.
#define MAXCHAR_PER_LINE 25
#define MAXQUEST 20

#include "QuestStatus.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/Actor.h"

UQuestButton::UQuestButton()
{
	SlotButton =CreateDefaultSubobject<UButton>(TEXT("SlotButton"));
	SlotText = CreateDefaultSubobject<URichTextBlock>(TEXT("SlotText"));
}

void UQuestButton::InitText(UDataTable* font)
{
	AddChild(SlotButton);
	AddChild(SlotText);
	Cast<UCanvasPanelSlot>(SlotButton->Slot)->SetSize(FVector2D(880.0f, 30.0f));
	SlotButton->OnPressed.AddDynamic(this, &UQuestButton::OnPressed_Broadcast);
	if (SlotText)
	{
		SlotText->SetTextStyleSet(font);
		SlotText->SetJustification(ETextJustify::Left);
		SlotText->SetText(FText::FromString("<QS>QuestButtonsssssssssssssssssss</>"));
	}
}

void UQuestButton::SetQuest(FQuest* quest)
{
	SlotText->SetText(FText::FromString(GetDisplayName(quest->Name)));
}

void UQuestButton::SetQsText(FString str)
{
	FString res = "<QS>" + str + "</>";
	SlotText->SetText(FText::FromString(res));
}

FString UQuestButton::GetDisplayName(FString name)
{
	int cnt = name.Len() / (MAXCHAR_PER_LINE + 1);
	for (int i = 0; i < cnt; i++)
	{
		name.InsertAt((i + 1) * MAXCHAR_PER_LINE + i, "\n");
	}

	return name;
}


void UQuestButton::OnPressed_Broadcast()
{
	PressedEvent.Broadcast(QuestIndex);
}

void UQuestStatus::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();

	if (!Bounded)
	{
		MainFont = Helpers::LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Font/DT_FontSet.DT_FontSet"));
		QuestSlotBrush_N.SetResourceObject(Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/QuestSlot_Normal.QuestSlot_Normal")));
		QuestSlotBrush_H.SetResourceObject(Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/QuestSlot_Hovered.QuestSlot_Hovered")));
		QuestSlotBrush_S.SetResourceObject(Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/QuestSlot_Selected.QuestSlot_Selected")));
		QuestSlotBrush_N.DrawAs = ESlateBrushDrawType::Image;
		QuestSlotBrush_H.DrawAs = ESlateBrushDrawType::Image;
		QuestSlotBrush_S.DrawAs = ESlateBrushDrawType::Image;

		RootCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("RootCanvas")));
		MainQuestScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("MainScroll")));
		QuestInfoBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("InfoBox")));

		if (RootCanvas && MainQuestScroll && QuestInfoBox)
		{
			// MainQuestList's Widgets
			for (int i = 0; i < MAXQUEST; i++)
			{
				MainQuestBlocks.Add(WidgetTree->ConstructWidget<UQuestButton>(UQuestButton::StaticClass(), FName(TEXT("MainQuestButton %d"), i)));
				MainQuestScroll->AddChild(MainQuestBlocks[i]);
				MainQuestBlocks[i]->InitText(MainFont);
				MainQuestBlocks[i]->SetRenderScale(FVector2D(1.0f, 2.0f));
				Cast<UScrollBoxSlot>(MainQuestBlocks[i]->Slot)->SetPadding(FMargin(0.0f, 30.0f, 0.0f, 0.0f));
				Cast<UCanvasPanelSlot>(MainQuestBlocks[i]->SlotText->Slot)->SetPosition(FVector2D(30.0f, -5.0f));
				MainQuestBlocks[i]->PressedEvent.AddDynamic(this, &UQuestStatus::OpenQuestInfo);
				MainQuestBlocks[i]->QuestIndex = i;

				MainQuestBlocks[i]->SlotButton->WidgetStyle.SetNormal(QuestSlotBrush_N);
				MainQuestBlocks[i]->SlotButton->WidgetStyle.SetHovered(QuestSlotBrush_H);
			}
			Cast<UScrollBoxSlot>(MainQuestBlocks[0]->Slot)->SetPadding(FMargin(0.0f, 30.0f, 0.0f, 0.0f));

			// QuestInfoBox's Widgets
			{
				QuestNameText = WidgetTree->ConstructWidget<URichTextBlock>(URichTextBlock::StaticClass(), TEXT("QuestName_TextBlock"));
				QuestInfoScroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("QuestInfo_ScrollBox"));
				QuestInfoText = WidgetTree->ConstructWidget<URichTextBlock>(URichTextBlock::StaticClass(), TEXT("QuestInfo_TextBlock"));
				SubQuestsScroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("SubQuests_ScrollBox"));
				SubQuestsText = WidgetTree->ConstructWidget<URichTextBlock>(URichTextBlock::StaticClass(), TEXT("SubQuests_RichTextBlock"));

				QuestInfoBox->AddChild(QuestNameText);
				QuestInfoBox->AddChild(QuestInfoScroll);
				QuestInfoBox->AddChild(SubQuestsScroll);

				FSlateChildSize slatechild;
				slatechild.SizeRule = ESlateSizeRule::Fill;
				slatechild.Value = 0.1f;
				Cast<UVerticalBoxSlot>(QuestNameText->Slot)->SetSize(FSlateChildSize(slatechild));
				Cast<UVerticalBoxSlot>(QuestNameText->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
				Cast<UVerticalBoxSlot>(QuestNameText->Slot)->SetPadding(FMargin(30.0f, 0.0f, 0.0f, 0.0f));
				slatechild.Value = 0.3f;
				Cast<UVerticalBoxSlot>(QuestInfoScroll->Slot)->SetSize(FSlateChildSize(slatechild));
				slatechild.Value = 0.6f;
				Cast<UVerticalBoxSlot>(SubQuestsScroll->Slot)->SetSize(FSlateChildSize(slatechild));

				QuestInfoScroll->AddChild(QuestInfoText);
				SubQuestsScroll->AddChild(SubQuestsText);

				QuestNameText->SetTextStyleSet(MainFont);

				QuestInfoText->SetTextStyleSet(MainFont);
				QuestInfoText->SetAutoWrapText(true);
				Cast<UScrollBoxSlot>(QuestInfoText->Slot)->SetPadding(FMargin(20.0f, 20.0f, 0.0f, 0.0f));

				SubQuestsText->SetTextStyleSet(MainFont);
				Cast<UScrollBoxSlot>(SubQuestsText->Slot)->SetPadding(FMargin(20.0f, 20.0f, 0.0f, 0.0f));


				QuestNameText->SetText(FText::FromString(Helpers::GetRichText("Quest Name", "InfoTitle")));
				QuestInfoText->SetText(FText::FromString(Helpers::GetRichText("Quest Info. This slot is for quest's information. And the reason I wroting this loooooong letter is for testing auto line breaking. Thanks!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", "InfoNormal")));
				SubQuestsText->SetText(FText::FromString(Helpers::GetRichText("SubQuest Text", "InfoNormal")));
			}
		}
		
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
	check(RootCanvas && MainQuestScroll && QuestInfoBox)

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

void UQuestStatus::RefreshBlocks()
{
	for (int i = 0; i < Player->WorkingQuests.Num(); i++)
	{
		MainQuestBlocks[i]->SetVisibility(ESlateVisibility::Visible);
		MainQuestBlocks[i]->SetQsText(Player->WorkingQuests[i]->Name);
		MainQuestBlocks[i]->SlotButton->WidgetStyle.SetNormal(QuestSlotBrush_N);
	}
	for (int i = Player->WorkingQuests.Num(); i < MAXQUEST; i++)
	{
		MainQuestBlocks[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	QuestNameText->SetText(FText::GetEmpty());
	QuestInfoText->SetText(FText::GetEmpty());
	SubQuestsText->SetText(FText::GetEmpty());
}

FString UQuestStatus::GetSubQuestText(FSingleQuest* subquest)
{
	FString header;
	if (subquest->Completed) header = "InfoStrike";
	else header = "InfoNormal";
	FString str = Helpers::GetRichText(subquest->Name, header);
	switch (subquest->Type)
	{
	case ESingleQuestType::Arrival:
		str += "\n";
		break;
	case ESingleQuestType::Hunt:
	{
		str += "\n";
		for (int i = 0; i < subquest->HuntingLists.Num(); i++)
		{
			FString subheader;
			if (subquest->HuntingLists[i].HuntAmount <= subquest->currAmounts[i]) subheader = "InfoStrike";
			else subheader = "InfoNormal";

			FString oneline;
			oneline += "\t";
			oneline += subquest->HuntingLists[i].Huntee->GetName();
			oneline += "   ";
			oneline += "(" + FString::FromInt(subquest->currAmounts[i]);
			oneline += "/" + FString::FromInt(subquest->HuntingLists[i].HuntAmount);
			oneline += ")";
			str += Helpers::GetRichText(oneline, subheader);
			str += "\n";
		}
		break;
	}
	case ESingleQuestType::Item:
	{
		str += "\n";
		for (int i = 0; i < subquest->ItemLists.Num(); i++)
		{
			FString subheader;
			if (subquest->ItemLists[i].ItemAmount <= subquest->currAmounts[i]) subheader = "InfoStrike";
			else subheader = "InfoNormal";

			FString oneline;
			oneline += "\t";
			oneline += subquest->ItemLists[i].ItemName;
			oneline += "   ";
			oneline += "(" + FString::FromInt(subquest->currAmounts[i]);
			oneline += "/" + FString::FromInt(subquest->ItemLists[i].ItemAmount);
			oneline += ")";
			str += Helpers::GetRichText(oneline, subheader);
			str += "\n";
		}
		break;
	}
	case ESingleQuestType::Action:
		str += "\n";
		break;
	default:
		break;
	}
	return str;
}

void UQuestStatus::OpenQuestInfo(int index)
{
	// Slot Selection
	if (SelectedSlotIndex >= 0)
	{
		MainQuestBlocks[SelectedSlotIndex]->SlotButton->WidgetStyle.SetNormal(QuestSlotBrush_N);
	}
	MainQuestBlocks[index]->SlotButton->WidgetStyle.SetNormal(QuestSlotBrush_S);
	SelectedSlotIndex = index;
	
	FQuest* Selected = Player->WorkingQuests[index];

	QuestNameText->SetText(FText::FromString("\n" + Selected->Name));
	QuestNameText->SetText(FText::FromString(Helpers::GetRichText(Selected->Name, "InfoTitle")));
	QuestInfoText->SetText(FText::FromString(Helpers::GetRichText(Selected->Info, "InfoNormal")));
	FString subquestString;

	if (Selected->Type == EQuestType::Serial)
	{
		for (int i = 0; i <= Selected->currPhase; i++)
		{
			if (i >= Selected->SubQuests.Num()) break;
			FString str = GetSubQuestText(&Selected->SubQuests[i]);
			subquestString += str;
			subquestString += "\n";
		}
	}
	else
	{
		for (int i = 0; i < Player->WorkingQuests[index]->SubQuests.Num(); i++)
		{
			FString str = GetSubQuestText(&Player->WorkingQuests[index]->SubQuests[i]);
			subquestString += str;
			subquestString += "\n";
		}
	}

	SubQuestsText->SetText(FText::FromString(subquestString));
}