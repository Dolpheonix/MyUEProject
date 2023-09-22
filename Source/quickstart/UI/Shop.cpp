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

		DefaultFont.FontObject = Helpers::LoadObjectFromPath<UObject>(TEXT("/Game/ShootingGame/Font/Ramche_Font.Ramche_Font"));
		DefaultFont.OutlineSettings.OutlineSize = 1;
		DefaultFont.Size = 28.0f;

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
			ShopSlots[i]->Index = i;
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
			CharacterSlots[i]->Index = i;
		}

		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_00"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_01"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_02"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_03"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_10"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_11"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_12"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_13"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_20"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_21"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_22"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_23"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_30"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_31"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_32"))));
		ShopSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopSlotNumber_33"))));

		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_00")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_01")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_02")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_03")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_10")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_11")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_12")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_13")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_20")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_21")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_22")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_23")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_30")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_31")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_32")));
		CharacterSlotNumbers.Add(Cast<UTextBlock>(GetWidgetFromName("CharacterSlotNumber_33")));

		TabSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Tab_Cloth"))));
		TabSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Tab_Weapon"))));
		TabSlots.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("Tab_Item"))));
		TabSlots[0]->Type = ETypeTag::Cloth;
		TabSlots[1]->Type = ETypeTag::Weapon;
		TabSlots[2]->Type = ETypeTag::Item;

		DecreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("Decrease")));
		IncreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("Increase")));

		ShopNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ShopName")));
		NumText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Num")));
		BuySellText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_BuySell")));
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

	for (int i = 0; i < ShopSlots.Num(); i++)
	{
		check(ShopSlots[i]);
		check(ShopSlotNumbers[i]);
	}
	for (int i = 0; i < CharacterSlots.Num(); i++)
	{
		check(CharacterSlots[i]);
		check(CharacterSlotNumbers[i]);
	}
	for (int i = 0; i < TabSlots.Num(); i++)
	{
		check(TabSlots[i]);
	}
	check(DecreaseButton);
	check(IncreaseButton);
	check(ShopNameText);
	check(NumText);
	check(BuySellText);
	check(PriceText);
	check(WalletText);
	check(ConfirmationButton);
	check(EndButton);
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
		if (TabSlots[i])
		{
			TabSlots[i]->SetClickMethod(EButtonClickMethod::DownAndUp);
			TabSlots[i]->ClickedEvent.AddDynamic(this, &UShop::TabCharacterSlot);
			TabSlots[i]->OnClicked.AddDynamic(this, &UShop::PlaySelectSound);
			TabSlots[i]->SetEvent();
		}
	}
	for (int i = 0; i < ShopSlots.Num(); i++)
	{
		if (ShopSlots[i])
		{
			ShopSlots[i]->ClickedEvent.AddDynamic(this, &UShop::SelectBuyItem);
			ShopSlots[i]->OnClicked.AddDynamic(this, &UShop::PlaySelectSound);
			ShopSlots[i]->SetEvent();
		}
	}
	for (int i = 0; i < CharacterSlots.Num(); i++)
	{
		if (CharacterSlots[i])
		{
			CharacterSlots[i]->ClickedEvent.AddDynamic(this, &UShop::SelectSellItem);
			CharacterSlots[i]->OnClicked.AddDynamic(this, &UShop::PlaySelectSound);
			CharacterSlots[i]->SetEvent();
		}
	}

	if(DecreaseButton) DecreaseButton->OnClicked.AddDynamic(this, &UShop::Decrease);
	if(IncreaseButton) IncreaseButton->OnClicked.AddDynamic(this, &UShop::Increase);

	if(ConfirmationButton) ConfirmationButton->OnClicked.AddDynamic(this, &UShop::Deal);

	if(EndButton) EndButton->OnClicked.AddDynamic(this, &UShop::EndShop);
}

void UShop::InitShop(ANPC* Interacted)
{
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Controller = Cast<APlayerController>(MainCharacter->GetController());

	// 화면 중앙에 마우스 커서 표시
	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	InteractedNPC = Interacted;
	SelectedItem = nullptr;
	CurrIndex = -1;

	ShopNameText->SetText(FText::FromString(InteractedNPC->DisplayName + TEXT("의 상점")));

	InitShopSlots();
	InitCharacterSlots();
	RefreshIncDec();
}

void UShop::InitShopSlots()
{
	for (int i = 0; i < InteractedNPC->ShopItems.Num(); i++)	// 상점 아이템을 상점 슬롯에 표시
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

	for (int i = InteractedNPC->ShopItems.Num(); i < ShopSlots.Num(); i++)	// 이외의 슬롯은 비활성화
	{
		ShopSlots[i]->SetStyle(DefaultStyle);		
		ShopSlots[i]->SetIsEnabled(false);

		ShopSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UShop::InitCharacterSlots()
{
	TArray<FItemForm>* Initslot = &MainCharacter->Inventory[(uint8)ETypeTag::Cloth].ItemForms;	// 초기화 시, 현재 탭은 의상 탭으로 설정
	for (int i = 0; i < Initslot->Num(); i++)	// 인벤토리 아이템을 아이템 슬롯에 표시
	{
		FSlateBrush brush_N, brush_H;
		brush_N.SetResourceObject((*Initslot)[i].Thumbnail_N);
		brush_H.SetResourceObject((*Initslot)[i].Thumbnail_H);
		CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
		CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
		CharacterSlots[i]->SetIsEnabled(true);
		CharacterSlots[i]->Type = ETypeTag::Cloth;

		CharacterSlotNumbers[i]->SetText(FText::FromString(FString::FromInt((*Initslot)[i].ShortForm.Num)));
		CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
	}

	for (int i = Initslot->Num(); i < CharacterSlots.Num(); i++)	// 이외의 슬롯은 비활성화
	{
		CharacterSlots[i]->SetStyle(DefaultStyle);
		CharacterSlots[i]->SetIsEnabled(false);

		CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrTab = ETypeTag::Cloth;
}

void UShop::TabCharacterSlot(int index, ETypeTag type)
{
	if (type == CurrTab)	// 현재 탭을 누르면 아무 일도 일어나지 않음
	{
		return;
	}
	else
	{
		TArray<FItemForm>* Currslot = &MainCharacter->Inventory[(uint8)type].ItemForms;	// 선택된 탭에 맞게 슬롯 수정
		for (int i = 0; i < Currslot->Num(); i++)
		{
			FSlateBrush brush_N, brush_H;
			brush_N.SetResourceObject((*Currslot)[i].Thumbnail_N);
			brush_H.SetResourceObject((*Currslot)[i].Thumbnail_H);
			CharacterSlots[i]->WidgetStyle.SetNormal(brush_N);
			CharacterSlots[i]->WidgetStyle.SetHovered(brush_H);
			CharacterSlots[i]->SetIsEnabled(true);
			CharacterSlots[i]->Type = type;

			CharacterSlotNumbers[i]->SetText(FText::FromString(FString::FromInt((*Currslot)[i].ShortForm.Num)));
			CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Visible);
		}

		for (int i = Currslot->Num(); i < CharacterSlots.Num(); i++)
		{
			CharacterSlots[i]->SetStyle(DefaultStyle);
			CharacterSlots[i]->SetIsEnabled(false);
			CharacterSlots[i]->Type = type;

			CharacterSlotNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	CurrTab = type;

	if (!bIsBuying)	// 판매 모드였다면, 인벤토리 슬롯의 첫 아이템을 판매 아이템으로 선택
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
	// 선택된 아이템이 있었다면, 선택을 취소
	if(CurrIndex >= 0) Unselect();

	// 누른 슬롯의 아이템을 선택
	SelectedItem = &InteractedNPC->ShopItems[index].ItemForm;
	FSlateBrush brush_N;
	brush_N.SetResourceObject(SelectedItem->Thumbnail_S);
	ShopSlots[index]->WidgetStyle.SetNormal(brush_N);

	// 구매 모드로 변경
	bIsBuying = true;
	CurrIndex = index;

	// 개수 초기화
	CurrNum = 1;
	RefreshIncDec();
	BuySellText->SetText(FText::FromString(TEXT("구매")));
}

void UShop::SelectSellItem(int index, ETypeTag type)
{
	// 선택한 아이템이 있었다면, 선택을 취소
	if (CurrIndex >= 0) Unselect();

	SelectedItem = &MainCharacter->Inventory[(uint8)type].ItemForms[index];	// 누른 슬롯의 아이템을 선택

	FSlateBrush brush_N;
	brush_N.SetResourceObject(SelectedItem->Thumbnail_S);
	CharacterSlots[index]->WidgetStyle.SetNormal(brush_N);

	// 판매 모드로 변경
	bIsBuying = false;
	CurrIndex = index;
	CurrTab = type;

	// 개수 초기화
	CurrNum = 1;
	RefreshIncDec();
	BuySellText->SetText(FText::FromString(TEXT("판매")));
}

void UShop::Deal()
{
	bool removed = false;
	if (bIsBuying)	// 구매
	{
		if (CurrPrice > MainCharacter->CurrMoney)	// 돈이 부족함
		{
			UE_LOG(LogTemp, Error, TEXT("No Money!"));
		}
		else
		{
			FString name = SelectedItem->ShortForm.NameTag;
			MainCharacter->CurrMoney -= CurrPrice;	// 플레이어의 돈을 차감

			// 플레이어의 인벤토리 로드
			ETypeTag type = SelectedItem->ShortForm.TypeTag;
			TArray<FItemForm>* Buyslot = &MainCharacter->Inventory[(uint8)type].ItemForms;

			// 이미 가지고 있는 아이템인지 검색
			int32 index = Buyslot->IndexOfByPredicate([name](const FItemForm& item)
				{
					return (item.ShortForm.NameTag == name);
				});

			// 새로 생기는 아이템 -> 인벤토리 끝에 추가 후 캐릭터 슬롯 새로고침
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
			// 보유 수량 0이면 제거 후 캐릭터 슬롯 새로고침
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
	else	// 판매
	{
		MainCharacter->CurrMoney += CurrPrice;	// 플레이어의 돈 증가

		SelectedItem->ShortForm.Num -= CurrNum;	// 아이템 개수 차감
		int32 num = SelectedItem->ShortForm.Num;

		if (num == 0)	// 아이템을 전부 팔았다면, 해당 아이템 인벤토리에서 삭제 후 캐릭터 슬롯 새로고침
		{
			MainCharacter->DeleteItem(CurrTab, CurrIndex);
			removed = true;

			RefreshCharacterSlots(CurrTab, CurrIndex);
		}
		else	// 남아있으면 개수만 변경
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

void UShop::EndShop()
{
	UGameplayStatics::PlaySound2D(this, ExitSound);
	InteractedNPC->UnInteract();
}

void UShop::Unselect()
{
	FItemForm unselected;

	if (bIsBuying)	// 구매 모드 ==> 선택한 아이템이 상점 슬롯에 있음
	{
		// 선택되었던 슬롯의 썸네일을 일반 썸네일로 변경 (선택 틀이 씌워지지 않은 썸네일)
		unselected = InteractedNPC->ShopItems[CurrIndex].ItemForm;
		FSlateBrush brush_N;
		brush_N.SetResourceObject(unselected.Thumbnail_N);
		ShopSlots[CurrIndex]->WidgetStyle.SetNormal(brush_N);
	}
	else	// 판매 모드 ==> 선택한 아이템이 캐릭터 슬롯에 있음
	{
		unselected = MainCharacter->Inventory[(uint8)CurrTab].ItemForms[CurrIndex];
		FSlateBrush brush_N;
		brush_N.SetResourceObject(unselected.Thumbnail_N);
		CharacterSlots[CurrIndex]->WidgetStyle.SetNormal(brush_N);
	}
}

void UShop::RefreshIncDec()
{
	if (SelectedItem)	// 선택한 아이템이 있다면
	{
		if (!SelectedItem->ShortForm.bIsSellable)	// 사고 팔 수 없는 아이템인 경우 ==> 개수 0으로 설정 후 버튼 비활성화
		{
			CurrNum = 0;
			CurrPrice = 0;
			IncreaseButton->SetIsEnabled(false);
			DecreaseButton->SetIsEnabled(false);
			ConfirmationButton->SetIsEnabled(false);
		}
		else
		{
			if (SelectedItem->ShortForm.Num < 1) // 아이템의 개수가 0개 ==> 버튼 비활성화
			{
				CurrNum = 0;
				IncreaseButton->SetIsEnabled(false);
				DecreaseButton->SetIsEnabled(false);
			}
			else if (SelectedItem->ShortForm.Num == 1)	// 아이템의 개수가 1개 ==> 증가/감소 버튼 비활성화
			{
				CurrNum = 1;
				IncreaseButton->SetIsEnabled(false);
				DecreaseButton->SetIsEnabled(false);
			}
			else if (CurrNum == 1)	// 현재 설정 개수가 1개 ==> 감소 버튼을 비활성화 
			{
				IncreaseButton->SetIsEnabled(true);
				DecreaseButton->SetIsEnabled(false);
			}
			else if (SelectedItem->ShortForm.Num == CurrNum)	// 현재 설정 개수가 최대 개수 ==> 증가 버튼을 비활성화
			{
				IncreaseButton->SetIsEnabled(false);
				DecreaseButton->SetIsEnabled(true);
			}
			else
			{
				IncreaseButton->SetIsEnabled(true);
				DecreaseButton->SetIsEnabled(true);
			}

			// 가격 설정
			if (bIsBuying)
			{
				CurrPrice = CurrNum * InteractedNPC->ShopItems[CurrIndex].Price;
			}
			else
			{
				// 판매의 경우, 아이템 종류에 따라 일괄 가격 설정
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

			// 살 수 있는 가격이면 구매 버튼 활성화
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
	WalletText->SetText(FText::FromString(TEXT("보유 : ") + FString::FromInt(MainCharacter->CurrMoney)));
}

void UShop::RefreshShopSlots(int changed)
{
	// 재고가 떨어진 아이템을 배열에서 제거하고, 슬롯을 재정렬
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
	// 아이템 개수 텍스트를 변경
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
