// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../Utils/Helpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

void UInventory::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
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

		DefaultFont.FontObject = Helpers::LoadObjectFromPath<UObject>(TEXT("/Game/ShootingGame/Font/8bitOperatorPlus-Bold_Font.8bitOperatorPlus-Bold_Font"));
		DefaultFont.OutlineSettings.OutlineSize = 1;
		DefaultFont.Size = 24.0f;

		PopupClass = StaticLoadClass(UUserWidget::StaticClass(), this, TEXT("/Game/ShootingGame/Blueprint/UI/PopUpUI.PopUpUI_C"));
		DeletePopup = nullptr;

		InitializeSlots();
		SetTexts();
	}
}

void UInventory::NativeConstruct()
{
	UUserWidget::NativeConstruct();
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	Controller = Cast<APlayerController>(Player->GetController());

	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	RefreshSlots();

	if (!Bounded)
	{
		SetEvents();
		Bounded = true;
	}

	TArray<AActor*> temp;
	UGameplayStatics::GetAllActorsOfClass(this, APreviewActor::StaticClass(), temp);
	if (temp.Num() == 1)
	{
		Preview = Cast<APreviewActor>(temp[0]);
		Preview->SyncEvent.Broadcast();
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
		UGameplayStatics::PlaySound2D(this, ExitSound);

		Player->GameMode->bShowroom = false;
		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
		Player->GameMode->ChangeMenuWidget(Player->GameMode->MainUI);
	}

	return FReply::Handled();
}

void UInventory::InitializeSlots()
{
	// Allocating Blueprint Image Widgets to Array
	{
	RootCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Root")));

	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_00"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_01"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_02"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_03"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_10"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_11"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_12"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_13"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_20"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_21"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_22"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_23"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_30"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_31"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_32"))));
	ClothSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Cloth_33"))));

	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_00"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_01"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_02"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_03"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_04"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_05"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_10"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_11"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_12"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_13"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_14"))));
	ItemSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Item_15"))));

	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_00"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_01"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_02"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_03"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_10"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_11"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_12"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_13"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_20"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_21"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_22"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_23"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_30"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_31"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_32"))));
	WeaponSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Weapon_33"))));

	TrashCan = Cast<UButton>(GetWidgetFromName(TEXT("Button_Trashcan")));
	CapturedImage = Cast<UImage>(GetWidgetFromName(TEXT("Captured")));
	}

	FSlateFontInfo LargeFont = DefaultFont;
	LargeFont.Size = 70.0f;

	WeaponText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Weapon_Text"));
	RootCanvas->AddChild(WeaponText);
	Cast<UCanvasPanelSlot>(WeaponText->Slot)->SetSize(FVector2D(300.0f, 100.0f));
	WeaponText->SetRenderTranslation(FVector2D(1400.0f, 20.0f));
	WeaponText->SetText(FText::FromString("Weapon"));
	WeaponText->SetFont(LargeFont);

	ItemText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Item_Text"));
	RootCanvas->AddChild(ItemText);
	Cast<UCanvasPanelSlot>(ItemText->Slot)->SetSize(FVector2D(300.0f, 100.0f));
	ItemText->SetRenderTranslation(FVector2D(1250.0f, 630.0f));
	ItemText->SetText(FText::FromString("Item"));
	ItemText->SetFont(LargeFont);

	ClothText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Cloth_Text"));
	RootCanvas->AddChild(ClothText);
	Cast<UCanvasPanelSlot>(ClothText->Slot)->SetSize(FVector2D(300.0f, 100.0f));
	ClothText->SetRenderTranslation(FVector2D(900.0f, 20.0f));
	ClothText->SetText(FText::FromString("Cloth"));
	ClothText->SetFont(LargeFont);


	for (int i = 0; i < 16; i++)
	{
		ClothSlots[i]->SetStyle(DefaultStyle);
		ClothSlots[i]->index = i;
		ClothSlots[i]->type = ETypeTag::Cloth;
		ClothSlots[i]->SetIsEnabled(false);

		WeaponSlots[i]->SetStyle(DefaultStyle);
		WeaponSlots[i]->index = i;
		WeaponSlots[i]->type = ETypeTag::Weapon;
		WeaponSlots[i]->SetIsEnabled(false);

		if (i < 12)
		{
			ItemSlots[i]->SetStyle(DefaultStyle);
			ItemSlots[i]->index = i;
			ItemSlots[i]->type = ETypeTag::Item;
			ItemSlots[i]->SetIsEnabled(false);
		}
	}

	for (int i = 0; i < WeaponSlots.Num(); i++)
	{
		int hor = i % 4;
		int ver = i / 4;

		WeaponSlotNumbers.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName("Weapon Slot " + i)));
		RootCanvas->AddChild(WeaponSlotNumbers[i]);
		Cast<UCanvasPanelSlot>(WeaponSlotNumbers[i]->Slot)->SetSize(FVector2D(25.0f, 40.0f));
		WeaponSlotNumbers[i]->SetRenderTranslation(FVector2D(1430.0f + hor * 125.0f, 160.0f + ver * 125.0f));
		WeaponSlotNumbers[i]->SetJustification(ETextJustify::Right);
		WeaponSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(0)));
		WeaponSlotNumbers[i]->SetFont(DefaultFont);
	}
	for (int i = 0; i < ItemSlots.Num(); i++)
	{
		int hor = i % 6;
		int ver = i / 6;

		ItemSlotNumbers.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName("Item Slot " + i)));
		RootCanvas->AddChild(ItemSlotNumbers[i]);
		Cast<UCanvasPanelSlot>(ItemSlotNumbers[i]->Slot)->SetSize(FVector2D(25.0f, 40.0f));
		ItemSlotNumbers[i]->SetRenderTranslation(FVector2D(1080.0f + hor * 125.0f, 760.0f + ver * 125.0f));
		ItemSlotNumbers[i]->SetJustification(ETextJustify::Right);
		ItemSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(0)));
		ItemSlotNumbers[i]->SetFont(DefaultFont);
	}
	for (int i = 0; i < ClothSlots.Num(); i++)
	{
		int hor = i % 4;
		int ver = i / 4;

		ClothSlotNumbers.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName("Cloth Slot " + i)));
		RootCanvas->AddChild(ClothSlotNumbers[i]);
		Cast<UCanvasPanelSlot>(ClothSlotNumbers[i]->Slot)->SetSize(FVector2D(25.0f, 40.0f));
		ClothSlotNumbers[i]->SetRenderTranslation(FVector2D(880.0f + hor * 125.0f, 160.0f + ver * 125.0f));
		ClothSlotNumbers[i]->SetJustification(ETextJustify::Right);
		ClothSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(0)));
		ClothSlotNumbers[i]->SetFont(DefaultFont);
	}

	CapturedImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	CapturedImage->SetOpacity(0.0f);
}

void UInventory::SetTexts()
{
	FSlateFontInfo InfoFont = DefaultFont;
	InfoFont.Size = 30.0f;

	InfoText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Info_Text"));
	RootCanvas->AddChild(InfoText);
	Cast<UCanvasPanelSlot>(InfoText->Slot)->SetSize(FVector2D(620.0f, 250.0f));
	InfoText->SetRenderTranslation(FVector2D(30.0f, 780.0f));
	InfoText->SetFont(InfoFont);
	InfoText->SetText(FText::FromString(""));
}

void UInventory::RefreshSlots()
{
	for (int i = 0; i < 16; i++)
	{
		if (i < Player->Inventory[(uint8)ETypeTag::Cloth].ItemForms.Num())
		{
			FSlateBrush brush_N, brush_H;
			if (i == Player->Quickslots_Now[(uint8)ETypeTag::Cloth]) brush_N.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].Thumbnail_S);
			else brush_N.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].Thumbnail_N);
			brush_H.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].Thumbnail_H);
			ClothSlots[i]->WidgetStyle.SetNormal(brush_N);
			ClothSlots[i]->WidgetStyle.SetHovered(brush_H);
			ClothSlots[i]->SetIsEnabled(true);

			ClothSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(Player->Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].ShortForm.Num)));
			ClothSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ClothSlots[i]->SetStyle(DefaultStyle);
			ClothSlots[i]->SetIsEnabled(false);

			ClothSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
		}

		if (i < Player->Inventory[(uint8)ETypeTag::Weapon].ItemForms.Num())
		{
			FSlateBrush brush_N, brush_H;
			if (i == Player->Quickslots_Now[(uint8)ETypeTag::Weapon]) brush_N.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].Thumbnail_S);
			else brush_N.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].Thumbnail_N);
			brush_H.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].Thumbnail_H);
			WeaponSlots[i]->WidgetStyle.SetNormal(brush_N);
			WeaponSlots[i]->WidgetStyle.SetHovered(brush_H);
			WeaponSlots[i]->SetIsEnabled(true);

			WeaponSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(Player->Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].ShortForm.Num)));
			WeaponSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			WeaponSlots[i]->SetStyle(DefaultStyle);
			WeaponSlots[i]->SetIsEnabled(false);

			WeaponSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		
		if (i < Player->Inventory[(uint8)ETypeTag::Item].ItemForms.Num())
		{
			FSlateBrush brush_N, brush_H;
			if (i == Player->Quickslots_Now[(uint8)ETypeTag::Item]) brush_N.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Item].ItemForms[i].Thumbnail_S);
			else brush_N.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Item].ItemForms[i].Thumbnail_N);
			brush_H.SetResourceObject(Player->Inventory[(uint8)ETypeTag::Item].ItemForms[i].Thumbnail_H);
			ItemSlots[i]->WidgetStyle.SetNormal(brush_N);
			ItemSlots[i]->WidgetStyle.SetHovered(brush_H);
			ItemSlots[i]->SetIsEnabled(true);

			ItemSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(Player->Inventory[(uint8)ETypeTag::Item].ItemForms[i].ShortForm.Num)));
			ItemSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else if (i < 12)
		{
			ItemSlots[i]->SetStyle(DefaultStyle);
			ItemSlots[i]->SetIsEnabled(false);

			ItemSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInventory::SetEvents()
{
	for (int i = 0; i < 16; i++)
	{
		ClothSlots[i]->HoveredEvent.AddDynamic(this, &UInventory::OnHovered_GetInfo);
		ClothSlots[i]->ClickedEvent.AddDynamic(this, &UInventory::OnClicked_Select);
		ClothSlots[i]->PressedEvent.AddDynamic(this, &UInventory::OnPressed_Catch);
		WeaponSlots[i]->HoveredEvent.AddDynamic(this, &UInventory::OnHovered_GetInfo);
		WeaponSlots[i]->ClickedEvent.AddDynamic(this, &UInventory::OnClicked_Select);
		WeaponSlots[i]->PressedEvent.AddDynamic(this, &UInventory::OnPressed_Catch);
		if (i < 12)
		{
			ItemSlots[i]->HoveredEvent.AddDynamic(this, &UInventory::OnHovered_GetInfo);
			ItemSlots[i]->ClickedEvent.AddDynamic(this, &UInventory::OnClicked_Select);
			ItemSlots[i]->PressedEvent.AddDynamic(this, &UInventory::OnPressed_Catch);
		}
	}
	for (int i = 0; i < 16; i++)
	{
		ClothSlots[i]->SetEvent();
		ClothSlots[i]->OnUnhovered.AddDynamic(this, &UInventory::OnUnhovered_DelInfo);
		ClothSlots[i]->OnReleased.AddDynamic(this, &UInventory::OnReleased_Delete);
		WeaponSlots[i]->SetEvent();
		WeaponSlots[i]->OnUnhovered.AddDynamic(this, &UInventory::OnUnhovered_DelInfo);
		WeaponSlots[i]->OnReleased.AddDynamic(this, &UInventory::OnReleased_Delete);
		if (i < 12)
		{
			ItemSlots[i]->SetEvent();
			ItemSlots[i]->OnUnhovered.AddDynamic(this, &UInventory::OnUnhovered_DelInfo);
			ItemSlots[i]->OnReleased.AddDynamic(this, &UInventory::OnReleased_Delete);
		}
	}
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

void UInventory::ChangeNumber(int index, ETypeTag type)
{
	FItemForm* ChangedItem = &Player->Inventory[(uint8)type].ItemForms[index];
	UTextBlock* toChange = nullptr;
	switch (type)
	{
	case ETypeTag::Cloth:
		toChange = ClothSlotNumbers[index];
		break;
	case ETypeTag::Weapon:
		toChange = WeaponSlotNumbers[index];
		break;
	case ETypeTag::Item:
		toChange = ItemSlotNumbers[index];
		break;
	default:
		break;
	}

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

	for (int i = 0; i < Player->Inventory[(uint8)ETypeTag::Weapon].ItemForms.Num(); i++)
	{
		WeaponSlots[i]->SetIsEnabled(false);
		list.Add((i + 1) * pow(10, (uint8)ETypeTag::Weapon));
	}
	for (int i = 0; i < Player->Inventory[(uint8)ETypeTag::Item].ItemForms.Num(); i++)
	{
		ItemSlots[i]->SetIsEnabled(false);
		list.Add((i + 1) * pow(10, (uint8)ETypeTag::Item));
	}
	for (int i = 0; i < Player->Inventory[(uint8)ETypeTag::Cloth].ItemForms.Num(); i++)
	{
		ClothSlots[i]->SetIsEnabled(false);
		list.Add((i + 1) * pow(10, (uint8)ETypeTag::Cloth));
	}

	ShutdownList = list;
}

void UInventory::RestoreButtons()
{
	TArray<int> div = { 1, 10, 100 };
	for (int idx : ShutdownList)
	{
		if (idx >= div[(uint8)ETypeTag::Cloth])
		{
			int index = idx / div[(uint8)ETypeTag::Cloth] - 1;
			ClothSlots[index]->SetIsEnabled(true);
		}
		else if (idx >= div[(uint8)ETypeTag::Item])
		{
			int index = idx / div[(uint8)ETypeTag::Item] - 1;
			ItemSlots[index]->SetIsEnabled(true);
		}
		else
		{
			int index = idx - 1;
			WeaponSlots[index]->SetIsEnabled(true);
		}
	}
}