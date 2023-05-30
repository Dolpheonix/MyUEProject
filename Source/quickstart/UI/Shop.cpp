// Fill out your copyright notice in the Description page of Project Settings.
///////////////////////////TODO///////////////////////
///////Sell, Buy 기능 추가//////////////

#include "Shop.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/Main/MainCharacter.h"

void UShop::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();

	bIsFocusable = true;

	if (!Bounded)
	{
		BuySellSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Item_Sell_Purchase_03_Cue.Item_Sell_Purchase_03_Cue"));
		IncreaseSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Abstract_Pop_05_Cue.Abstract_Pop_05_Cue"));
		DecreaseSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Abstract_Pop_02_Cue.Abstract_Pop_02_Cue"));
		SelectSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Pop_01_Cue.Pop_01_Cue"));
		ExitSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Futuristic_Click_07_Cue.Futuristic_Click_07_Cue"));

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

		for (int i = 0; i < ShopSlots.Num(); i++)
		{
			ShopSlots[i]->index = i;
		}

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

		for (int i = 0; i < CharacterSlots.Num(); i++)
		{
			CharacterSlots[i]->index = i;
		}

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
		PriceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Price")));
		WalletText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Wallet")));

		ConfirmationButton = Cast<UButton>(GetWidgetFromName(TEXT("Confirmation")));

		EndButton = Cast<UButton>(GetWidgetFromName(TEXT("End")));

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
		TabSlots[i]->OnClicked.AddDynamic(this, &UShop::PlaySelectSound);
		TabSlots[i]->SetEvent();
	}
	for (int i = 0; i < ShopSlots.Num(); i++)
	{
		ShopSlots[i]->ClickedEvent.AddDynamic(this, &UShop::SelectBuyItem);
		ShopSlots[i]->OnClicked.AddDynamic(this, &UShop::PlaySelectSound);
		ShopSlots[i]->SetEvent();
	}
	for (int i = 0; i < CharacterSlots.Num(); i++)
	{
		CharacterSlots[i]->ClickedEvent.AddDynamic(this, &UShop::SelectSellItem);
		CharacterSlots[i]->OnClicked.AddDynamic(this, &UShop::PlaySelectSound);
		CharacterSlots[i]->SetEvent();
	}

	DecreaseButton->OnClicked.AddDynamic(this, &UShop::Decrease);
	IncreaseButton->OnClicked.AddDynamic(this, &UShop::Increase);

	ConfirmationButton->OnClicked.AddDynamic(this, &UShop::Deal);

	EndButton->OnClicked.AddDynamic(this, &UShop::EndShop);
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

	CurrIndex = -1;

	InitShopSlots();
	InitCharacterSlots();

	if (InteractedNPC->ShopItems.Num() > 0)
	{
		SelectBuyItem(0, ETypeTag::Cloth);
	}

	WalletText->SetText(FText::FromString(FString::FromInt(MainCharacter->CurrMoney)));
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
		ShopSlots[i]->SetIsEnabled(true);
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
		ShopSlots[i]->SetIsEnabled(false);
	}
}

void UShop::InitCharacterSlots()
{
	TArray<FItemForm>* Initslot = &MainCharacter->Inventory[(uint8)ETypeTag::Cloth].ItemForms;
	for (int i = 0; i < Initslot->Num(); i++)
	{
		FSlateBrush brush_N, brush_H;
		brush_N.SetResourceObject((*Initslot)[i].Thumbnail_N);
		brush_H.SetResourceObject((*Initslot)[i].Thumbnail_H);
		CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
		CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
		CharacterSlots[i]->SetIsEnabled(true);
	}

	UTexture2D* defaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
	FButtonStyle DefaultStyle;
	FSlateBrush DefaultBrush;
	DefaultBrush.SetResourceObject(defaultTex);
	DefaultStyle.SetNormal(DefaultBrush);
	DefaultStyle.SetHovered(DefaultBrush);

	for (int i = Initslot->Num(); i < CharacterSlots.Num(); i++)
	{
		CharacterSlots[i]->SetStyle(DefaultStyle);
		CharacterSlots[i]->SetIsEnabled(false);
	}

	CurrTab = ETypeTag::Cloth;
}

void UShop::TabCharacterSlot(int index, ETypeTag type)
{
	if (type == CurrTab)
	{
		return;
	}
	else
	{
		TArray<FItemForm>* Currslot = &MainCharacter->Inventory[(uint8)type].ItemForms;
		for (int i = 0; i < Currslot->Num(); i++)
		{
			FSlateBrush brush_N, brush_H;
			brush_N.SetResourceObject((*Currslot)[i].Thumbnail_N);
			brush_H.SetResourceObject((*Currslot)[i].Thumbnail_H);
			CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
			CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
			CharacterSlots[i]->SetIsEnabled(true);
			CharacterSlots[i]->type = type;
		}

		UTexture2D* defaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
		FButtonStyle DefaultStyle;
		FSlateBrush DefaultBrush;
		DefaultBrush.SetResourceObject(defaultTex);
		DefaultStyle.SetNormal(DefaultBrush);
		DefaultStyle.SetHovered(DefaultBrush);

		for (int i = Currslot->Num(); i < CharacterSlots.Num(); i++)
		{
			CharacterSlots[i]->SetStyle(DefaultStyle);
			CharacterSlots[i]->SetIsEnabled(false);
			CharacterSlots[i]->type = type;
		}
	}

	CurrTab = type;

	if (!bIsBuying)
	{
		CurrIndex = -1;
		SelectSellItem(0, type);
	}
}

void UShop::Increase()
{
	CurrNum++;
	RefreshIncDec();
	UGameplayStatics::PlaySound2D(this, IncreaseSound);
}

void UShop::Decrease()
{
	CurrNum--;
	RefreshIncDec();
	UGameplayStatics::PlaySound2D(this, DecreaseSound);
}

void UShop::SelectBuyItem(int index, ETypeTag type)
{
	// Unselect
	if(CurrIndex >= 0) Unselect();

	// Select
	SelectedItem = &InteractedNPC->ShopItems[index];
	FSlateBrush brush_N;
	brush_N.SetResourceObject(SelectedItem->Thumbnail_S);
	ShopSlots[index]->WidgetStyle.SetNormal(brush_N);

	// Current State Transition
	bIsBuying = true;
	CurrIndex = index;

	CurrNum = 1;
	RefreshIncDec();
	BuySellText->SetText(FText::FromString("Buy"));
}

void UShop::SelectSellItem(int index, ETypeTag type)
{
	if (CurrIndex >= 0) Unselect();

	SelectedItem = &MainCharacter->Inventory[(uint8)type].ItemForms[index];

	FSlateBrush brush_N;
	brush_N.SetResourceObject(SelectedItem->Thumbnail_S);
	CharacterSlots[index]->WidgetStyle.SetNormal(brush_N);

	bIsBuying = false;
	CurrIndex = index;
	CurrTab = type;

	CurrNum = 1;
	RefreshIncDec();
	BuySellText->SetText(FText::FromString("Sell"));
}

void UShop::Deal()
{
	bool removed = false;
	if (bIsBuying)
	{
		///BUY
		if (CurrPrice > MainCharacter->CurrMoney)
		{
			UE_LOG(LogTemp, Error, TEXT("No Money!"));
		}
		else
		{
			// MainCharacter 지불
			FString name = SelectedItem->ShortForm.NameTag;
			MainCharacter->CurrMoney -= CurrPrice;

			// MainCharacter에 구매한 아이템 추가
			ETypeTag type = SelectedItem->ShortForm.TypeTag;
			TArray<FItemForm>* Buyslot = &MainCharacter->Inventory[(uint8)type].ItemForms;

			// 이미 가지고 있는 아이템인지 검색
			int32 index = Buyslot->IndexOfByPredicate([name](const FItemForm& item)
				{
					return (item.ShortForm.NameTag == name);
				});
			// 새로 생기는 아이템. 인벤토리 끝에 추가
			if (index < 0)
			{
				index = Buyslot->Num();
				FItemShortForm Receipt = SelectedItem->ShortForm;
				Receipt.Num = CurrNum;
				MainCharacter->Register(Receipt);
			}
			// 이미 있는 아이템. 개수만 추가
			else
			{
				(*Buyslot)[index].ShortForm.Num += CurrNum;
			}

			// NPC 슬롯에서 차감
			SelectedItem->ShortForm.Num -= CurrNum;

			if (SelectedItem->ShortForm.Num == 0)
			{
				InteractedNPC->ShopItems.RemoveAt(CurrIndex);
				InteractedNPC->Prices.RemoveAt(CurrIndex);
				removed = true;
			}
			// 상점 새로고침
			RefreshShopSlots(CurrIndex);

			if (type == CurrTab)
			{
				RefreshCharacterSlots(CurrTab, index);
			}
		}
	}
	else
	{
		// Not sellable
		///SELLable
		MainCharacter->CurrMoney += CurrPrice;

		SelectedItem->ShortForm.Num -= CurrNum;
		int32 num = SelectedItem->ShortForm.Num;

		if (num == 0)
		{
			MainCharacter->DeleteItem(CurrTab, CurrIndex);
			removed = true;
		}

		RefreshCharacterSlots(CurrTab, CurrIndex);
	}

	if (removed)
	{
		if (InteractedNPC->ShopItems.Num() > 0)
		{
			CurrIndex = -1;
			SelectBuyItem(0, ETypeTag::Cloth);
		}
		else
		{
			CurrNum = 0;
			CurrPrice = 0;
			CurrIndex = -1;
			IncreaseButton->SetIsEnabled(false);
			DecreaseButton->SetIsEnabled(false);
			ConfirmationButton->SetIsEnabled(false);
			NumText->SetText(FText::FromString(FString::FromInt(CurrNum)));
			PriceText->SetText(FText::FromString(FString::FromInt(CurrPrice)));
			WalletText->SetText(FText::FromString(FString::FromInt(MainCharacter->CurrMoney)));
		}
	}
	else
	{
		if (bIsBuying) SelectBuyItem(CurrIndex, ETypeTag::Cloth);
		else		  SelectSellItem(CurrIndex, CurrTab);
	}

	UGameplayStatics::PlaySound2D(this, BuySellSound);
}

void UShop::EndShop()
{
	UGameplayStatics::PlaySound2D(this, ExitSound);
	InteractedNPC->UnInteract();
}

void UShop::Unselect()
{
	FItemForm unselected;

	if (bIsBuying)
	{
		unselected = InteractedNPC->ShopItems[CurrIndex];
		FSlateBrush brush_N;
		brush_N.SetResourceObject(unselected.Thumbnail_N);
		ShopSlots[CurrIndex]->WidgetStyle.SetNormal(brush_N);
	}
	else
	{
		unselected = MainCharacter->Inventory[(uint8)CurrTab].ItemForms[CurrIndex];
		FSlateBrush brush_N;
		brush_N.SetResourceObject(unselected.Thumbnail_N);
		CharacterSlots[CurrIndex]->WidgetStyle.SetNormal(brush_N);
	}
}

void UShop::RefreshIncDec()
{
	if (!SelectedItem->ShortForm.bIsSellable)
	{
		CurrNum = 0;
		CurrPrice = 0;
		IncreaseButton->SetIsEnabled(false);
		DecreaseButton->SetIsEnabled(false);
		ConfirmationButton->SetIsEnabled(false);
	}
	else
	{
		if (SelectedItem->ShortForm.Num < 1) // No Item left
		{
			CurrNum = 0;
			IncreaseButton->SetIsEnabled(false);
			DecreaseButton->SetIsEnabled(false);
		}
		else if (SelectedItem->ShortForm.Num == 1)
		{
			CurrNum = 1;
			IncreaseButton->SetIsEnabled(false);
			DecreaseButton->SetIsEnabled(false);
		}
		else if (CurrNum == 1)
		{
			IncreaseButton->SetIsEnabled(true);
			DecreaseButton->SetIsEnabled(false);
		}
		else if (SelectedItem->ShortForm.Num == CurrNum)
		{
			IncreaseButton->SetIsEnabled(false);
			DecreaseButton->SetIsEnabled(true);
		}
		else
		{
			IncreaseButton->SetIsEnabled(true);
			DecreaseButton->SetIsEnabled(true);
		}

		// Price
		if (bIsBuying)
		{
			CurrPrice = CurrNum * InteractedNPC->Prices[CurrIndex];
		}
		else
		{
			switch (CurrTab)
			{
			case ETypeTag::Cloth:
				CurrPrice = CurrNum * 100;
				break;
			case ETypeTag::Weapon:
				CurrPrice = CurrNum * 70;
				break;
			case ETypeTag::Item:
				CurrPrice = CurrNum * 20;
				break;
			default:
				break;
			}
		}

		// Payable
		if (MainCharacter->CurrMoney <= CurrPrice)
		{
			ConfirmationButton->SetIsEnabled(false);
		}
		else
		{
			ConfirmationButton->SetIsEnabled(true);
		}
	}
	NumText->SetText(FText::FromString(FString::FromInt(CurrNum)));
	PriceText->SetText(FText::FromString(FString::FromInt(CurrPrice)));
	WalletText->SetText(FText::FromString(FString::FromInt(MainCharacter->CurrMoney)));
}

void UShop::RefreshShopSlots(int changed)
{
	// 재고가 떨어진 아이템을 array에서 제거하고, 슬롯을 재정렬
	for (int i = changed; i < InteractedNPC->ShopItems.Num(); i++)
	{
		FSlateBrush brush_N, brush_H;
		brush_N.SetResourceObject(InteractedNPC->ShopItems[i].Thumbnail_N);
		brush_H.SetResourceObject(InteractedNPC->ShopItems[i].Thumbnail_H);
		ShopSlots[i]->WidgetStyle.SetNormal(brush_N);
		ShopSlots[i]->WidgetStyle.SetHovered(brush_H);
		ShopSlots[i]->SetIsEnabled(true);
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
		ShopSlots[i]->SetIsEnabled(false);
	}
}

void UShop::RefreshCharacterSlots(ETypeTag type, int changed)
{
	TArray<FItemForm>* arr = &MainCharacter->Inventory[(uint8)type].ItemForms;

	for (int i = changed; i < arr->Num(); i++)
	{
		FSlateBrush brush_N, brush_H;
		brush_N.SetResourceObject((*arr)[i].Thumbnail_N);
		brush_H.SetResourceObject((*arr)[i].Thumbnail_H);
		CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
		CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
		CharacterSlots[i]->SetIsEnabled(true);
	}

	UTexture2D* defaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
	FButtonStyle DefaultStyle;
	FSlateBrush DefaultBrush;
	DefaultBrush.SetResourceObject(defaultTex);
	DefaultStyle.SetNormal(DefaultBrush);
	DefaultStyle.SetHovered(DefaultBrush);

	for (int i = arr->Num(); i < CharacterSlots.Num(); i++)
	{
		CharacterSlots[i]->SetStyle(DefaultStyle);
		CharacterSlots[i]->SetIsEnabled(false);
	}
}

void UShop::PlaySelectSound()
{
	UGameplayStatics::PlaySound2D(this, SelectSound);
}
