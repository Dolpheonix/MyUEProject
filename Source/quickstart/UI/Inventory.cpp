// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../Utils/Helpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

void UInventory::NativePreConstruct()
{
	Super::NativePreConstruct();
	bIsFocusable = true;
	if (!Bounded)
	{
		SelectSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Pop_01_Cue.Pop_01_Cue"));
		CatchSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Click_01_Cue.Click_01_Cue"));
		PopupSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Coins_01_Cue.Coins_01_Cue"));
		ExitSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Click_02_Cue.Click_02_Cue"));

		DefaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
		DefaultBrush.SetResourceObject(DefaultTex);
		DefaultStyle.SetNormal(DefaultBrush);
		DefaultStyle.SetHovered(DefaultBrush);

		DefaultFont.FontObject = Helpers::LoadObjectFromPath<UObject>(TEXT("/Game/ShootingGame/Font/Ramche_Font.Ramche_Font"));
		DefaultFont.OutlineSettings.OutlineSize = 1;
		DefaultFont.Size = 24.0f;

		PopupClass = StaticLoadClass(UUserWidget::StaticClass(), this, TEXT("/Game/ShootingGame/Blueprint/UI/PopUpUI.PopUpUI_C"));
		DeletePopup = nullptr;

		InfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Info")));

		InitializeSlots();
		SetEvents();

		Bounded = true;
	}
}

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	Controller = Cast<APlayerController>(Player->GetController());

	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	RefreshSlots();

	TArray<AActor*> temp;
	UGameplayStatics::GetAllActorsOfClass(this, APreviewActor::StaticClass(), temp);
	if (temp.Num() == 1)
	{
		Preview = Cast<APreviewActor>(temp[0]);
		Preview->SyncEvent.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("You must place one PreviewActor at one map"));
	}
}

void UInventory::NativeDestruct()
{
	Controller->SetShowMouseCursor(false);

	UUserWidget::NativeDestruct();
}

void UInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	if (isCaptured)
	{
		FVector2D Pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		Pos.X -= 50.0f;
		Pos.Y -= 50.0f;
		CapturedImage->SetRenderTranslation(Pos);
	}
}

FReply UInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	int ascii = InKeyEvent.GetCharacter();
	if (ascii == 73 || ascii == 105)
	{
		Exit();
	}

	return FReply::Handled();
}

void UInventory::Exit()
{
	UGameplayStatics::PlaySound2D(this, ExitSound);

	Player->GameMode->bShowroom = false;
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());
	Player->GameMode->ChangeMenuWidget(Player->GameMode->MainUI);
}

void UInventory::InitializeSlots()
{
	// Allocating Blueprint Image Widgets to Array
	{
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_00"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_01"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_02"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_03"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_10"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_11"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_12"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_13"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_20"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_21"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_22"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_23"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_30"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_31"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_32"))));
		ItemButtons.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemButton_33"))));

		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_00"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_01"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_02"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_03"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_10"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_11"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_12"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_13"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_20"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_21"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_22"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_23"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_30"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_31"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_32"))));
		ItemNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNumber_33"))));

		ItemTabs.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemTab_Cloth"))));
		ItemTabs.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemTab_Weapon"))));
		ItemTabs.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemTab_Item"))));
		
		TabTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TabText_Cloth"))));
		TabTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TabText_Weapon"))));
		TabTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TabText_Item"))));

		TrashCan = Cast<UButton>(GetWidgetFromName(TEXT("Button_Trashcan")));
		CapturedImage = Cast<UImage>(GetWidgetFromName(TEXT("Captured")));

		ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Exit")));

		CurrMoneyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrMoney")));
	}

	for (int i = 0; i < 16; i++)
	{
		ItemButtons[i]->SetStyle(DefaultStyle);
		ItemButtons[i]->Index = i;
		ItemButtons[i]->SetIsEnabled(false);
	}

	ItemTabs[0]->Type = ETypeTag::Cloth;
	ItemTabs[1]->Type = ETypeTag::Weapon;
	ItemTabs[2]->Type = ETypeTag::Item;

	CurrTab = ETypeTag::Cloth;
	CurrTabText = TabTexts[0];
	ItemTabs[0]->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f));

	CapturedImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	CapturedImage->SetOpacity(0.0f);
}

void UInventory::RefreshSlots()
{
	for (int i = 0; i < 16; i++)
	{
		ItemButtons[i]->Type = CurrTab;

		FSlateBrush NBrush, HBrush;
		if (i < Player->Inventory[(uint8)CurrTab].ItemForms.Num())
		{
			if (i == Player->Quickslots_Now[(uint8)CurrTab]) NBrush.SetResourceObject(Player->Inventory[(uint8)CurrTab].ItemForms[i].Thumbnail_S);
			else NBrush.SetResourceObject(Player->Inventory[(uint8)CurrTab].ItemForms[i].Thumbnail_N);

			HBrush.SetResourceObject(Player->Inventory[(uint8)CurrTab].ItemForms[i].Thumbnail_H);

			ItemButtons[i]->WidgetStyle.SetNormal(NBrush);
			ItemButtons[i]->WidgetStyle.SetHovered(HBrush);
			ItemButtons[i]->WidgetStyle.SetPressed(NBrush);
			ItemButtons[i]->SetIsEnabled(true);

			ItemNumbers[i]->SetText(FText::FromString(FString::FromInt(Player->Inventory[(uint8)CurrTab].ItemForms[i].ShortForm.Num)));
			ItemNumbers[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemButtons[i]->SetStyle(DefaultStyle);
			ItemButtons[i]->SetIsEnabled(false);

			ItemNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
		}

		CurrMoneyText->SetText(FText::FromString(TEXT("º¸À¯ : ") + FString::FromInt(Player->CurrMoney)));
	}
}

void UInventory::SetEvents()
{
	for (int i = 0; i < 16; i++)
	{
		ItemButtons[i]->HoveredEvent.AddDynamic(this, &UInventory::OnHovered_GetInfo);
		ItemButtons[i]->ClickedEvent.AddDynamic(this, &UInventory::OnClicked_Select);
		ItemButtons[i]->PressedEvent.AddDynamic(this, &UInventory::OnPressed_Catch);
		ItemButtons[i]->OnUnhovered.AddDynamic(this, &UInventory::OnUnhovered_DelInfo);
		ItemButtons[i]->OnReleased.AddDynamic(this, &UInventory::OnReleased_Delete);
		ItemButtons[i]->SetEvent();
	}

	for (int i = 0; i < 3; i++)
	{
		ItemTabs[i]->ClickedEvent.AddDynamic(this, &UInventory::OnClicked_Tab);
		ItemTabs[i]->SetEvent();
	}

	ExitButton->OnClicked.AddDynamic(this, &UInventory::Exit);
}

void UInventory::OpenPopup()
{
	if (!DeletePopup && PopupClass)
	{
		DeletePopup = WidgetTree->ConstructWidget<UPopUp>(PopupClass, TEXT("PopUp"));
		RootCanvas->AddChild(DeletePopup);
		DeletePopup->ConfirmButton->OnPressed.AddDynamic(this, &UInventory::ThrowAway);
		DeletePopup->CancleButton->OnPressed.AddDynamic(this, &UInventory::RestoreButtons);
	}
	else
	{
		RootCanvas->AddChild(DeletePopup);
	}

	FAnchors anchor;
	anchor.Maximum = FVector2D(0.5f, 0.5f);
	anchor.Minimum = FVector2D(0.5f, 0.5f);
	Cast<UCanvasPanelSlot>(DeletePopup->Slot)->SetAnchors(anchor);
	DeletePopup->InitializeNumber(Player->Inventory[(uint8)CapturedInfo.ItemType].ItemForms[CapturedInfo.index].ShortForm.Num);

	DisableButtonsTemporarily();
}

void UInventory::OnHovered_GetInfo(int index, ETypeTag type)
{
	TArray<FItemForm>* Currslot = &Player->Inventory[(uint8)type].ItemForms;
	if (index < Currslot->Num())
	{
		InfoText->SetText(FText::FromString((*Currslot)[index].ShortForm.InfoTag));
	}
}

void UInventory::OnUnhovered_DelInfo()
{
	InfoText->SetText(FText::FromString(""));
}

void UInventory::OnClicked_Select(int index, ETypeTag type)
{
	TArray<FItemForm>* Currslot = &Player->Inventory[(uint8)type].ItemForms;
	if (index < Currslot->Num())
	{
		Player->Quickslots_Before[(uint8)type] = Player->Quickslots_Now[(uint8)type];
		Player->Quickslots_Now[(uint8)type] = index;
		Player->RefreshInventory(type);
		RefreshSlots();

		if (type == ETypeTag::Weapon)
		{
			Player->unEquip();
			Player->Equip();
		}
		if (type != ETypeTag::Item)
		{
			Preview->Sync_to_Character();
		}
	}

	UGameplayStatics::PlaySound2D(this, SelectSound);
}

void UInventory::OnPressed_Catch(int index, ETypeTag type)
{
	FVector2D Pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	Pos.X -= 50.0f;
	Pos.Y -= 50.0f;

	TArray<FItemForm>* Currslot = &Player->Inventory[(uint8)type].ItemForms;
	if (index < Currslot->Num())
	{
		isCaptured = true;
		CapturedInfo.index = index;
		CapturedInfo.ItemType = type;
		CapturedInfo.tag = (*Currslot)[index].ShortForm.NameTag;
		CapturedImage->SetOpacity(0.7f);
		CapturedImage->SetBrushFromTexture((*Currslot)[index].Thumbnail_N);
		CapturedImage->SetRenderTranslation(Pos);
	}

	UGameplayStatics::PlaySound2D(this, CatchSound);
}

void UInventory::OnReleased_Delete()
{
	FVector2D Pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	if (isCaptured)
	{
		if (Pos.X > 740.0f && Pos.X < 940.0f && Pos.Y > 770.0f && Pos.Y < 970.0f && CapturedInfo.index > 0)
		{
			OpenPopup();
			UGameplayStatics::PlaySound2D(this, PopupSound);
		}
		else
		{
			UGameplayStatics::PlaySound2D(this, CatchSound);
		}
		isCaptured = false;
		CapturedImage->SetOpacity(0.0f);
		CapturedImage->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	}
}

void UInventory::OnClicked_Tab(int index, ETypeTag type)
{
	switch (CurrTab)
	{
	case ETypeTag::Cloth:
		ItemTabs[0]->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
		break;
	case ETypeTag::Weapon:
		ItemTabs[1]->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
		break;
	case ETypeTag::Item:
		ItemTabs[2]->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
		break;
	default:
		break;
	}

	CurrTab = type;
	
	switch(CurrTab)
	{
	case ETypeTag::Cloth:
		ItemTabs[0]->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f));
		break;
	case ETypeTag::Weapon:
		ItemTabs[1]->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f));
		break;
	case ETypeTag::Item:
		ItemTabs[2]->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f));
		break;
	default:
		break;
	}

	RefreshSlots();
}

void UInventory::ChangeNumber(int index, ETypeTag type)
{
	FItemForm* ChangedItem = &Player->Inventory[(uint8)type].ItemForms[index];
	UTextBlock* toChange = ItemNumbers[index];

	if(toChange) toChange->SetText(FText::FromString(FString::FromInt(ChangedItem->ShortForm.Num)));
}

void UInventory::ThrowAway()
{
	if (DeletePopup)
	{
		FItemForm* Selected = &Player->Inventory[(uint8)CapturedInfo.ItemType].ItemForms[CapturedInfo.index];
		Player->ReportItem(Selected->ShortForm.NameTag, -DeletePopup->CurrNum);
		if (DeletePopup->CurrNum < DeletePopup->MaxNum)
		{
			Selected->ShortForm.Num -= DeletePopup->CurrNum;
			ChangeNumber(CapturedInfo.index, CapturedInfo.ItemType);
			RestoreButtons();
		}
		else
		{
			Player->DeleteItem(CapturedInfo.ItemType, CapturedInfo.index);
			Preview->Delete_and_Sync(CapturedInfo.ItemType, CapturedInfo.tag);
			RefreshSlots();
		}

		DeletePopup->RemoveFromParent();
	}
}

void UInventory::DisableButtonsTemporarily()
{
	TArray<int> list;

	for (int i = 0; i < Player->Inventory[(uint8)CurrTab].ItemForms.Num(); i++)
	{
		ItemButtons[i]->SetIsEnabled(false);
		list.Add(i);
	}

	ShutdownList = list;
}

void UInventory::RestoreButtons()
{
	for (int idx : ShutdownList)
	{
		ItemButtons[idx]->SetIsEnabled(true);
	}
}