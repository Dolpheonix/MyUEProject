// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "../Utils/Helpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UInventory::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();

	bIsFocusable = true;
	InitializeSlots();
	SetTexts();
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
	SetEvents();

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

	defaultSlotTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
	FButtonStyle DefaultStyle;
	FSlateBrush DefaultBrush;
	DefaultBrush.SetResourceObject(defaultSlotTex);
	DefaultStyle.SetNormal(DefaultBrush);
	DefaultStyle.SetHovered(DefaultBrush);

	for (int i = 0; i < 16; i++)
	{
		ClothSlots[i]->SetStyle(DefaultStyle);
		ClothSlots[i]->index = i;
		ClothSlots[i]->type = ETypeTag::Cloth;
		WeaponSlots[i]->SetStyle(DefaultStyle);
		WeaponSlots[i]->index = i;
		WeaponSlots[i]->type = ETypeTag::Weapon;
		if (i < 12)
		{
			ItemSlots[i]->SetStyle(DefaultStyle);
			ItemSlots[i]->index = i;
			ItemSlots[i]->type = ETypeTag::Item;
		}
	}

	CapturedImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	CapturedImage->SetOpacity(0.0f);
}

void UInventory::SetTexts()
{
	TextBlocks.Add(Cast<UTextBlock>(GetWidgetFromName("Cloth_Text")));
	TextBlocks.Add(Cast<UTextBlock>(GetWidgetFromName("Weapon_Text")));
	TextBlocks.Add(Cast<UTextBlock>(GetWidgetFromName("Item_Text")));
	InfoTextBlock = Cast<UTextBlock>(GetWidgetFromName("Info_Text"));

	TextFont = Helpers::LoadObjectFromPath<UObject>("/Game/ShootingGame/Font/OldLondon_Font.OldLondon_Font");
	FSlateFontInfo FontInfo;
	FontInfo.FontObject = TextFont;
	FontInfo.Size = 70;


	for (int i = 0; i < 3; i++)
	{
		TextBlocks[i]->SetFont(FontInfo);
	}

	FontInfo.Size = 30;
	InfoTextBlock->SetFont(FontInfo);

	TextBlocks[0]->SetText(FText::FromString("Cloth"));
	TextBlocks[1]->SetText(FText::FromString("Weapon"));
	TextBlocks[2]->SetText(FText::FromString("Item"));
	InfoTextBlock->SetText(FText::FromString(""));
}

void UInventory::RefreshSlots()
{
	FButtonStyle DefaultStyle;
	FSlateBrush DefaultBrush;
	DefaultBrush.SetResourceObject(defaultSlotTex);
	DefaultStyle.SetNormal(DefaultBrush);
	DefaultStyle.SetHovered(DefaultBrush);

	for (int i = 0; i < 16; i++)
	{
		if (i < Player->Clothes.Num())
		{
			FSlateBrush brush_N, brush_H;
			if (i == Player->Cloth_Now) brush_N.SetResourceObject(Player->Clothes[i].Thumbnail_S);
			else brush_N.SetResourceObject(Player->Clothes[i].Thumbnail_N);
			brush_H.SetResourceObject(Player->Clothes[i].Thumbnail_H);
			ClothSlots[i]->WidgetStyle.SetNormal(brush_N);
			ClothSlots[i]->WidgetStyle.SetHovered(brush_H);
		}
		else
		{
			ClothSlots[i]->SetStyle(DefaultStyle);
		}

		if (i < Player->Weapons.Num())
		{
			FSlateBrush brush_N, brush_H;
			if (i == Player->Weapon_Now) brush_N.SetResourceObject(Player->Weapons[i].Thumbnail_S);
			else brush_N.SetResourceObject(Player->Weapons[i].Thumbnail_N);
			brush_H.SetResourceObject(Player->Weapons[i].Thumbnail_H);
			WeaponSlots[i]->WidgetStyle.SetNormal(brush_N);
			WeaponSlots[i]->WidgetStyle.SetHovered(brush_H);
		}
		else
		{
			WeaponSlots[i]->SetStyle(DefaultStyle);
		}
		
		if (i < Player->Items.Num())
		{
			FSlateBrush brush_N, brush_H;
			if (i == Player->Item_Now) brush_N.SetResourceObject(Player->Items[i].Thumbnail_S);
			else brush_N.SetResourceObject(Player->Items[i].Thumbnail_N);
			brush_H.SetResourceObject(Player->Items[i].Thumbnail_H);
			ItemSlots[i]->WidgetStyle.SetNormal(brush_N);
			ItemSlots[i]->WidgetStyle.SetHovered(brush_H);
		}
		else if (i < 12)
		{
			ItemSlots[i]->SetStyle(DefaultStyle);
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

void UInventory::OnHovered_GetInfo(int index, ETypeTag type)
{
	if (type == ETypeTag::Cloth && index < Player->Clothes.Num())
	{
		InfoTextBlock->SetText(FText::FromString(Player->Clothes[index].ItemInfo));
	}
	else if (type == ETypeTag::Weapon && index < Player->Weapons.Num())
	{
		InfoTextBlock->SetText(FText::FromString(Player->Weapons[index].ItemInfo));
	}
	else if (index < Player->Items.Num())
	{
		InfoTextBlock->SetText(FText::FromString(Player->Items[index].ItemInfo));
	}
}

void UInventory::OnUnhovered_DelInfo()
{
	InfoTextBlock->SetText(FText::FromString(""));
}

void UInventory::OnClicked_Select(int index, ETypeTag type)
{
	if (type == ETypeTag::Cloth && index < Player->Clothes.Num())
	{
		Player->Cloth_Now = index;
		Player->RefreshInventory(ETypeTag::Cloth);
		RefreshSlots();
		Preview->Sync_to_Character();
	}
	else if (type == ETypeTag::Weapon && index < Player->Weapons.Num())
	{
		Player->Weapon_Before = Player->Weapon_Now;
		Player->Weapon_Now = index;
		Player->RefreshInventory(ETypeTag::Weapon);
		Player->unEquip();
		Player->Equip();
		RefreshSlots();
		Preview->Sync_to_Character();
	}
	else if (index < Player->Items.Num())
	{
		Player->Item_Now = index;
		Player->RefreshInventory(ETypeTag::Item);
		RefreshSlots();
	}
}

void UInventory::OnPressed_Catch(int index, ETypeTag type)
{
	FVector2D Pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	Pos.X -= 50.0f;
	Pos.Y -= 50.0f;
	if (type == ETypeTag::Cloth && index < Player->Clothes.Num())
	{
		isCaptured = true;
		CapturedInfo.index = index;
		CapturedInfo.ItemType = type;
		CapturedInfo.tag = Player->Clothes[index].NameTag;
		CapturedImage->SetOpacity(0.7f);
		CapturedImage->SetBrushFromTexture(Player->Clothes[index].Thumbnail_N);
		CapturedImage->SetRenderTranslation(Pos);
	}
	else if (type == ETypeTag::Weapon && index < Player->Weapons.Num())
	{
		isCaptured = true;
		CapturedInfo.index = index;
		CapturedInfo.ItemType = type;
		CapturedInfo.tag = Player->Weapons[index].NameTag;
		CapturedImage->SetOpacity(0.7f);
		CapturedImage->SetBrushFromTexture(Player->Weapons[index].Thumbnail_N);
		CapturedImage->SetRenderTranslation(Pos);
	}
	else if (index < Player->Items.Num())
	{
		isCaptured = true;
		CapturedInfo.index = index;
		CapturedInfo.ItemType = type;
		CapturedInfo.tag = Player->Items[index].NameTag;
		CapturedImage->SetOpacity(0.7f);
		CapturedImage->SetBrushFromTexture(Player->Items[index].Thumbnail_N);
		CapturedImage->SetRenderTranslation(Pos);
	}
}

void UInventory::OnReleased_Delete()
{
	FVector2D Pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	if (isCaptured)
	{
		if (Pos.X > 740.0f && Pos.X < 940.0f && Pos.Y > 770.0f && Pos.Y < 970.0f)
		{
			Player->DeleteItem(CapturedInfo.ItemType, CapturedInfo.index);
			RefreshSlots();
			Preview->Delete_and_Sync(CapturedInfo.ItemType, CapturedInfo.tag);
		}
		else
		{

		}
		isCaptured = false;
		CapturedImage->SetOpacity(0.0f);
		CapturedImage->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	}
}