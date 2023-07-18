// Fill out your copyright notice in the Description page of Project Settings.
///////////////////////////TODO///////////////////////
///////Sell, Buy 기능 추가//////////////

#include "Shop.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/Main/MainCharacter.h"

void UShop::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();

	bIsFocusable = true;

	if (!Bounded)
	{
		BuySellSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Item_Sell_Purchase_01_Cue.Item_Sell_Purchase_01_Cue"));
		IncreaseSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Pop_03_Cue.Pop_03_Cue"));
		DecreaseSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Pop_02_Cue.Pop_02_Cue"));
		SelectSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Pop_01_Cue.Pop_01_Cue"));
		ExitSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Click_02_Cue.Click_02_Cue"));

		DefaultTex = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Empty_Normal.Empty_Normal"));
		DefaultBrush.SetResourceObject(DefaultTex);
		DefaultStyle.SetNormal(DefaultBrush);
		DefaultStyle.SetHovered(DefaultBrush);

		DefaultFont.FontObject = Helpers::LoadObjectFromPath<UObject>(TEXT("/Game/ShootingGame/Font/OldLondon_Font.OldLondon_Font"));
		DefaultFont.OutlineSettings.OutlineSize = 1;
		DefaultFont.Size = 28.0f;

		RootCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Root")));

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

		for (int i = 0; i < ShopSlots.Num(); i++)
		{
			int hor = i % 4;
			int ver = i / 4;
			ShopSlotNumbers.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName("ShopSlot Number " + i)));
			RootCanvas->AddChild(ShopSlotNumbers[i]);
			Cast<UCanvasPanelSlot>(ShopSlotNumbers[i]->Slot)->SetSize(FVector2D(25.0f, 40.0f));
			ShopSlotNumbers[i]->SetRenderTranslation(FVector2D(210.0f + hor * 150.0f, 210.0f + ver * 150.0f));
			ShopSlotNumbers[i]->SetJustification(ETextJustify::Right);
			ShopSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(0)));
			ShopSlotNumbers[i]->SetFont(DefaultFont);
		}

		for (int i = 0; i < CharacterSlots.Num(); i++)
		{
			int hor = i % 4;
			int ver = i / 4;
			CharacterSlotNumbers.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName("CharacterSlot Number " + i)));
			RootCanvas->AddChild(CharacterSlotNumbers[i]);
			Cast<UCanvasPanelSlot>(CharacterSlotNumbers[i]->Slot)->SetSize(FVector2D(25.0f, 40.0f));
			CharacterSlotNumbers[i]->SetRenderTranslation(FVector2D(1310.0f + hor * 150.0f, 220.0f + ver * 150.0f));
			CharacterSlotNumbers[i]->SetJustification(ETextJustify::Right);
			CharacterSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(0)));
			CharacterSlotNumbers[i]->SetFont(DefaultFont);
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

// Player와의 대화를 통해 상점 인터페이스가 열릴 때 호출됨
// 마우스 커서, 입력 모드 설정
// 상점 슬롯, 캐릭터 슬롯 초기화
// 버튼, 수량, 가격 등 초기화
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
	SelectedItem = nullptr;
	CurrIndex = -1;

	InitShopSlots();
	InitCharacterSlots();
	RefreshIncDec();
}

// 상점 슬롯 초기화, 버튼 활성화 여부 결정
void UShop::InitShopSlots()
{
	for (int i = 0; i < InteractedNPC->ShopItems.Num(); i++)
	{
		FSlateBrush brush_N, brush_H;
		brush_N.SetResourceObject(InteractedNPC->ShopItems[i].ItemForm.Thumbnail_N);
		brush_H.SetResourceObject(InteractedNPC->ShopItems[i].ItemForm.Thumbnail_H);
		ShopSlots[i]->WidgetStyle.SetNormal(brush_N);
		ShopSlots[i]->WidgetStyle.SetHovered(brush_H);
		ShopSlots[i]->SetIsEnabled(true);

		ShopSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(InteractedNPC->ShopItems[i].ItemForm.ShortForm.Num)));
		ShopSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
	}

	for (int i = InteractedNPC->ShopItems.Num(); i < ShopSlots.Num(); i++)
	{
		ShopSlots[i]->SetStyle(DefaultStyle);		
		ShopSlots[i]->SetIsEnabled(false);

		ShopSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

// 캐릭터 슬롯 초기화, 버튼 활성화 여부 결정
// 의상 탭으로 초기화
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
		CharacterSlots[i]->type = ETypeTag::Cloth;

		CharacterSlotNumbers[i]->SetText(FText::FromString(FString::FromInt((*Initslot)[i].ShortForm.Num)));
		CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
	}

	for (int i = Initslot->Num(); i < CharacterSlots.Num(); i++)
	{
		CharacterSlots[i]->SetStyle(DefaultStyle);
		CharacterSlots[i]->SetIsEnabled(false);

		CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrTab = ETypeTag::Cloth;
}

// 탭 버튼을 눌렀을 때 호출됨.
// 캐릭터 슬롯을 눌린 탭 타입의 아이템들로 초기화
// 만약 캐릭터 슬롯을 선택 중이었다면 바뀐 탭의 첫번째 아이템을 선택시킴 (단, Unselect할 필요는 없으므로 CurrIndex는 -1로 설정 후 선택)
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

			CharacterSlotNumbers[i]->SetText(FText::FromString(FString::FromInt((*Currslot)[i].ShortForm.Num)));
			CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
		}

		for (int i = Currslot->Num(); i < CharacterSlots.Num(); i++)
		{
			CharacterSlots[i]->SetStyle(DefaultStyle);
			CharacterSlots[i]->SetIsEnabled(false);
			CharacterSlots[i]->type = type;

			CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	CurrTab = type;

	if (!bIsBuying)
	{
		CurrIndex = -1;
		SelectSellItem(0, type);
	}
}

// 증가 버튼을 눌렀을 때 호출
// 수량을 증가시킨 후, RefreshIncDec()호출
void UShop::Increase()
{
	CurrNum++;
	RefreshIncDec();
	UGameplayStatics::PlaySound2D(this, IncreaseSound);
}

// 감소 버튼을 눌렀을 때 호출
// 수량을 감소시킨 후, RefreshIncDec()호출
void UShop::Decrease()
{
	CurrNum--;
	RefreshIncDec();
	UGameplayStatics::PlaySound2D(this, DecreaseSound);
}

// 상점 슬롯을 눌렀을 때 호출
// 기존에 선택되었던 아이템 Unselect
// SelectedItem 설정 후 버튼 이미지 변경
// 수량을 1로 조정 후 RefreshIncDec() 호출
void UShop::SelectBuyItem(int index, ETypeTag type)
{
	// Unselect
	if(CurrIndex >= 0) Unselect();

	// Select
	SelectedItem = &InteractedNPC->ShopItems[index].ItemForm;
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

// 캐릭터 슬롯을 눌렀을 때 호출
// 기존에 선택되었던 아이템 Unselect
// SelectedItem 설정 후 버튼 이미지 변경
// 수량을 1로 조정 후 RefreshIncDec() 호출
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

// 구매/판매 버튼을 눌렀을 때 호출
// 구매
//		플레이어의 돈에서 차감
//		가지고 있던 아이템이면 보유 개수만 변경, 아니면 인벤토리에 추가
//		상점 슬롯에서 보유 수량 변경 후 0이면 슬롯에서 제거
//		슬롯에 변경점 있으면 RefreshSlot() 아니면 ChangeNumber()
// 판매
//		플레이어의 돈에 추가
//		인벤토리에서 보유 수량 차감 후 0이면 인벤토리에서 제거
//		슬롯에 변경점 있으면 RefreshSlot() 아니면 ChangeNumber()
// 선택된 슬롯이 제거됨 --> SelectedItem nullptr 지정
// 제거되지 않음 --> 구매/판매 수량 1로 재지정
// RefreshIncDec() 호출
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

			// 새로 생기는 아이템 -> 인벤토리 끝에 추가 후 캐릭터 슬롯 Refresh
			if (index < 0)
			{
				index = Buyslot->Num();
				FItemShortForm Receipt = SelectedItem->ShortForm;
				Receipt.Num = CurrNum;
				MainCharacter->Register(Receipt);

				if(type == CurrTab) RefreshCharacterSlots(CurrTab, index);
			}
			// 이미 있는 아이템 -> 개수만 추가, 변경
			else
			{
				(*Buyslot)[index].ShortForm.Num += CurrNum;

				if (type == CurrTab) ChangeNumber(false, index);
			}

			// NPC 슬롯에서 차감
			SelectedItem->ShortForm.Num -= CurrNum;
			// 보유 수량 0이면 제거 후 캐릭터 슬롯 초기화
			if (SelectedItem->ShortForm.Num == 0)
			{
				InteractedNPC->ShopItems.RemoveAt(CurrIndex);
				removed = true;

				RefreshShopSlots(CurrIndex);
			}
			// 남아있으면 개수만 변경
			else
			{
				ChangeNumber(true, CurrIndex);
			}
		}
	}
	else
	{
		MainCharacter->CurrMoney += CurrPrice;

		SelectedItem->ShortForm.Num -= CurrNum;
		int32 num = SelectedItem->ShortForm.Num;

		if (num == 0)
		{
			MainCharacter->DeleteItem(CurrTab, CurrIndex);
			removed = true;

			RefreshCharacterSlots(CurrTab, CurrIndex);
		}
		else
		{
			ChangeNumber(false, CurrIndex);
		}
	}

	if (removed)
	{
		SelectedItem = nullptr;
		CurrIndex = -1;
	}
	else
	{
		CurrNum = 1;
	}

	RefreshIncDec();

	UGameplayStatics::PlaySound2D(this, BuySellSound);
}

// Exit 버튼 눌렀을 때 호출
// NPC와의 상호작용을 종료
void UShop::EndShop()
{
	UGameplayStatics::PlaySound2D(this, ExitSound);
	InteractedNPC->UnInteract();
}

// Select() 호출 시에 이전에 선택된 슬롯을 Unselect
void UShop::Unselect()
{
	FItemForm unselected;

	if (bIsBuying)
	{
		unselected = InteractedNPC->ShopItems[CurrIndex].ItemForm;
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

// SelectedItem에 따라
// Increase, Decrease, Confirmation Button의 활성 여부와
// Buy number, Price, Wallet의 값을 변경
void UShop::RefreshIncDec()
{
	if (SelectedItem)
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
				CurrPrice = CurrNum * InteractedNPC->ShopItems[CurrIndex].Price;
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
			if (bIsBuying && MainCharacter->CurrMoney < CurrPrice)
			{
				ConfirmationButton->SetIsEnabled(false);
			}
			else
			{
				ConfirmationButton->SetIsEnabled(true);
			}
		}
	}
	else // 아무것도 선택하지 않은 경우
	{
		CurrNum = 0;
		CurrPrice = 0;
		IncreaseButton->SetIsEnabled(false);
		DecreaseButton->SetIsEnabled(false);
		ConfirmationButton->SetIsEnabled(false);
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
		brush_N.SetResourceObject(InteractedNPC->ShopItems[i].ItemForm.Thumbnail_N);
		brush_H.SetResourceObject(InteractedNPC->ShopItems[i].ItemForm.Thumbnail_H);
		ShopSlots[i]->WidgetStyle.SetNormal(brush_N);
		ShopSlots[i]->WidgetStyle.SetHovered(brush_H);
		ShopSlots[i]->SetIsEnabled(true);

		ShopSlotNumbers[i]->SetText(FText::FromString(FString::FromInt(InteractedNPC->ShopItems[i].ItemForm.ShortForm.Num)));
		ShopSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
	}

	for (int i = InteractedNPC->ShopItems.Num(); i < ShopSlots.Num(); i++)
	{
		ShopSlots[i]->SetStyle(DefaultStyle);
		ShopSlots[i]->SetIsEnabled(false);

		ShopSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
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

		CharacterSlotNumbers[i]->SetText(FText::FromString(FString::FromInt((*arr)[i].ShortForm.Num)));
		CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
	}

	for (int i = arr->Num(); i < CharacterSlots.Num(); i++)
	{
		CharacterSlots[i]->SetStyle(DefaultStyle);
		CharacterSlots[i]->SetIsEnabled(false);

		CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UShop::ChangeNumber(bool isShopslot, int index)
{
	if (isShopslot)
	{
		ShopSlotNumbers[index]->SetText(FText::FromString(FString::FromInt(InteractedNPC->ShopItems[index].ItemForm.ShortForm.Num)));
	}
	else
	{
		CharacterSlotNumbers[index]->SetText(FText::FromString(FString::FromInt(MainCharacter->Inventory[(uint8)CurrTab].ItemForms[index].ShortForm.Num)));
	}
}

void UShop::PlaySelectSound()
{
	UGameplayStatics::PlaySound2D(this, SelectSound);
}
