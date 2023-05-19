// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/Main/MainCharacter.h"

void UShop::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();

	bIsFocusable = true;

	if (!Bounded)
	{
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_00"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_01"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_02"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_03"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_10"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_11"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_12"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_13"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_20"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_21"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_22"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_23"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_30"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_31"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_32"))));
		ShopSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Shop_33"))));

		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_00"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_01"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_02"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_03"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_10"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_11"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_12"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_13"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_20"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_21"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_22"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_23"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_30"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_31"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_32"))));
		CharacterSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Character_33"))));

		TabSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Tab_Cloth"))));
		TabSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Tab_Weapon"))));
		TabSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Tab_Item"))));
		TabSlots[0]->type = ETypeTag::Cloth;
		TabSlots[1]->type = ETypeTag::Weapon;
		TabSlots[2]->type = ETypeTag::Item;

		DecreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("Decrease")));
		IncreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("Increase")));

		NumText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Num")));
		BuySellText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_BuyorSell")));
		
		EndButton = Cast<UButton>(GetWidgetFromName(TEXT("End")));
		EndButton->OnClicked.AddDynamic(this, &UShop::EndShop);

		SetEvents();

		Bounded = true;
	}
}

void UShop::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void UShop::NativeDestruct()
{
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());

	InteractedNPC = nullptr;

	UUserWidget::NativeDestruct();
}

void UShop::SetEvents()
{
	for (int i = 0; i < TabSlots.Num(); i++)
	{
		TabSlots[i]->SetClickMethod(EButtonClickMethod::DownAndUp);
		TabSlots[i]->ClickedEvent.AddDynamic(this, &UShop::TabCharacterSlot);
		TabSlots[i]->SetEvent();
	}
}

void UShop::InitShop(ANPC* Interacted)
{
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Controller = Cast<APlayerController>(MainCharacter->GetController());

	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	InteractedNPC = Interacted;

	InitShopSlots();
	InitCharacterSlots();
}

void UShop::InitShopSlots()
{
	for (int i = 0; i < InteractedNPC->ShopItems.Num(); i++)
	{
		FSlateBrush brush_N, brush_H;
		brush_N.SetResourceObject(InteractedNPC->ShopItems[i].Thumbnail_N);
		brush_H.SetResourceObject(InteractedNPC->ShopItems[i].Thumbnail_H);
		ShopSlots[i]->WidgetStyle.SetNormal(brush_N);
		ShopSlots[i]->WidgetStyle.SetHovered(brush_H);
	}

	UTexture2D* defaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
	FButtonStyle DefaultStyle;
	FSlateBrush DefaultBrush;
	DefaultBrush.SetResourceObject(defaultTex);
	DefaultStyle.SetNormal(DefaultBrush);
	DefaultStyle.SetHovered(DefaultBrush);

	for (int i = InteractedNPC->ShopItems.Num(); i < ShopSlots.Num(); i++)
	{
		ShopSlots[i]->SetStyle(DefaultStyle);
	}
}

void UShop::InitCharacterSlots()
{
	for (int i = 0; i < MainCharacter->Clothes.Num(); i++)
	{
		FSlateBrush brush_N, brush_H;
		brush_N.SetResourceObject(MainCharacter->Clothes[i].Thumbnail_N);
		brush_H.SetResourceObject(MainCharacter->Clothes[i].Thumbnail_H);
		CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
		CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
	}

	UTexture2D* defaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
	FButtonStyle DefaultStyle;
	FSlateBrush DefaultBrush;
	DefaultBrush.SetResourceObject(defaultTex);
	DefaultStyle.SetNormal(DefaultBrush);
	DefaultStyle.SetHovered(DefaultBrush);

	for (int i = MainCharacter->Clothes.Num(); i < CharacterSlots.Num(); i++)
	{
		CharacterSlots[i]->SetStyle(DefaultStyle);
	}

	CurrTab = ETypeTag::Cloth;
}

void UShop::TabCharacterSlot(int index, ETypeTag type)
{
	if (type == CurrTab)
	{
		return;
	}
	else if (type == ETypeTag::Cloth)
	{
		for (int i = 0; i < MainCharacter->Clothes.Num(); i++)
		{
			FSlateBrush brush_N, brush_H;
			brush_N.SetResourceObject(MainCharacter->Clothes[i].Thumbnail_N);
			brush_H.SetResourceObject(MainCharacter->Clothes[i].Thumbnail_H);
			CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
			CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
		}

		UTexture2D* defaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
		FButtonStyle DefaultStyle;
		FSlateBrush DefaultBrush;
		DefaultBrush.SetResourceObject(defaultTex);
		DefaultStyle.SetNormal(DefaultBrush);
		DefaultStyle.SetHovered(DefaultBrush);

		for (int i = MainCharacter->Clothes.Num(); i < CharacterSlots.Num(); i++)
		{
			CharacterSlots[i]->SetStyle(DefaultStyle);
		}
	}
	else if (type == ETypeTag::Weapon)
	{
		for (int i = 0; i < MainCharacter->Weapons.Num(); i++)
		{
			FSlateBrush brush_N, brush_H;
			brush_N.SetResourceObject(MainCharacter->Weapons[i].Thumbnail_N);
			brush_H.SetResourceObject(MainCharacter->Weapons[i].Thumbnail_H);
			CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
			CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
		}

		UTexture2D* defaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
		FButtonStyle DefaultStyle;
		FSlateBrush DefaultBrush;
		DefaultBrush.SetResourceObject(defaultTex);
		DefaultStyle.SetNormal(DefaultBrush);
		DefaultStyle.SetHovered(DefaultBrush);

		for (int i = MainCharacter->Weapons.Num(); i < CharacterSlots.Num(); i++)
		{
			CharacterSlots[i]->SetStyle(DefaultStyle);
		}
	}
	else if (type == ETypeTag::Item)
	{
		for (int i = 0; i < MainCharacter->Items.Num(); i++)
		{
			FSlateBrush brush_N, brush_H;
			brush_N.SetResourceObject(MainCharacter->Items[i].Thumbnail_N);
			brush_H.SetResourceObject(MainCharacter->Items[i].Thumbnail_H);
			CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
			CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
		}

		UTexture2D* defaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
		FButtonStyle DefaultStyle;
		FSlateBrush DefaultBrush;
		DefaultBrush.SetResourceObject(defaultTex);
		DefaultStyle.SetNormal(DefaultBrush);
		DefaultStyle.SetHovered(DefaultBrush);

		for (int i = MainCharacter->Items.Num(); i < CharacterSlots.Num(); i++)
		{
			CharacterSlots[i]->SetStyle(DefaultStyle);
		}
	}

	CurrTab = type;
}

void UShop::EndShop()
{
	InteractedNPC->UnInteract();
}