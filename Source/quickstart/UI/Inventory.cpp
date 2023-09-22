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

	// 화면 중앙에 마우스 커서 표시
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
		Preview->SyncEvent.Broadcast();	// 플레이어와 프리뷰 액터를 동기화
	}
	else	// 프리뷰 액터가 존재하지 않거나, 2개 이상 존재
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

	if (isCaptured)	// 캡쳐된 슬롯이 있을 경우
	{
		FVector2D Pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		Pos.X -= 50.0f;
		Pos.Y -= 50.0f;
		CapturedImage->SetRenderTranslation(Pos);	// 캡쳐 이미지가 마우스를 따라다니게 함
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

	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());
	Player->GameMode->ChangeCurrentWidget(Player->GameMode->MainUI);	// 기본 UI로 변경
}

void UInventory::InitializeSlots()
{
	{
		// 아이템 슬롯 버튼
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
		// 아이템 개수 텍스트 블록
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
		// 아이템 탭 버튼
		ItemTabs.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemTab_Cloth"))));
		ItemTabs.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemTab_Weapon"))));
		ItemTabs.Add(Cast<UItemButton>(GetWidgetFromName(TEXT("ItemTab_Item"))));
		// 아이템 탭 텍스트 블록
		TabTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TabText_Cloth"))));
		TabTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TabText_Weapon"))));
		TabTexts.Add(Cast<UTextBlock>(GetWidgetFromName(TEXT("TabText_Item"))));
		// 휴지통 버튼
		TrashCan = Cast<UButton>(GetWidgetFromName(TEXT("Button_Trashcan")));
		// 캡쳐 이미지
		CapturedImage = Cast<UImage>(GetWidgetFromName(TEXT("Captured")));
		// Exit 버튼
		ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Exit")));
		// 현재 Money 텍스트 블록
		CurrMoneyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrMoney")));
	}

	// 슬롯 버튼을 기본 이미지로 초기화
	for (int i = 0; i < 16; i++)
	{
		ItemButtons[i]->SetStyle(DefaultStyle);
		ItemButtons[i]->Index = i;
		ItemButtons[i]->SetIsEnabled(false);
	}

	// 탭 버튼의 타입을 지정
	ItemTabs[0]->Type = ETypeTag::Cloth;
	ItemTabs[1]->Type = ETypeTag::Weapon;
	ItemTabs[2]->Type = ETypeTag::Item;

	CurrTab = ETypeTag::Cloth;	// 처음 인벤토리를 열었을 때, Cloth 탭이 먼저 열림
	CurrTabText = TabTexts[0];
	ItemTabs[0]->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f));

	CapturedImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	CapturedImage->SetOpacity(0.0f);
}

void UInventory::RefreshSlots()
{
	for (int i = 0; i < 16; i++)
	{
		ItemButtons[i]->Type = CurrTab;	// 현재 탭의 타입으로 슬롯의 타입을 변경

		FSlateBrush NBrush, HBrush;
		if (i < Player->Inventory[(uint8)CurrTab].ItemForms.Num())
		{
			if (i == Player->Quickslots_Now[(uint8)CurrTab]) NBrush.SetResourceObject(Player->Inventory[(uint8)CurrTab].ItemForms[i].Thumbnail_S);
			else NBrush.SetResourceObject(Player->Inventory[(uint8)CurrTab].ItemForms[i].Thumbnail_N);

			HBrush.SetResourceObject(Player->Inventory[(uint8)CurrTab].ItemForms[i].Thumbnail_H);

			ItemButtons[i]->WidgetStyle.SetNormal(NBrush);
			ItemButtons[i]->WidgetStyle.SetHovered(HBrush);
			ItemButtons[i]->WidgetStyle.SetPressed(NBrush);
			ItemButtons[i]->SetIsEnabled(true);	// 슬롯의 버튼 기능 활성화

			ItemNumbers[i]->SetText(FText::FromString(FString::FromInt(Player->Inventory[(uint8)CurrTab].ItemForms[i].ShortForm.Num)));
			ItemNumbers[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else	// 빈 아이템 슬롯
		{
			ItemButtons[i]->SetStyle(DefaultStyle);
			ItemButtons[i]->SetIsEnabled(false);

			ItemNumbers[i]->SetVisibility(ESlateVisibility::Hidden);
		}

		CurrMoneyText->SetText(FText::FromString(TEXT("보유 : ") + FString::FromInt(Player->CurrMoney)));	// 보유 Money 표시
	}
}

void UInventory::SetEvents()
{
	for (int i = 0; i < 16; i++)	// 아이템 슬롯 버튼
	{
		ItemButtons[i]->HoveredEvent.AddDynamic(this, &UInventory::OnHovered_GetInfo);	// Hovered 이벤트
		ItemButtons[i]->ClickedEvent.AddDynamic(this, &UInventory::OnClicked_Select);	// Clicked 이벤트	
		ItemButtons[i]->PressedEvent.AddDynamic(this, &UInventory::OnPressed_Catch);	// Pressed 이벤트
		ItemButtons[i]->OnUnhovered.AddDynamic(this, &UInventory::OnUnhovered_DelInfo);	// UnHovered 이벤트
		ItemButtons[i]->OnReleased.AddDynamic(this, &UInventory::OnReleased_Delete);	// Released 이벤트
		ItemButtons[i]->SetEvent();
	}

	for (int i = 0; i < 3; i++)	// 아이템 탭 버튼
	{
		ItemTabs[i]->ClickedEvent.AddDynamic(this, &UInventory::OnClicked_Tab);
		ItemTabs[i]->SetEvent();
	}

	ExitButton->OnClicked.AddDynamic(this, &UInventory::Exit);
}

void UInventory::OpenPopup()
{
	if (!DeletePopup && PopupClass)	// 팝업 UI를 새로 생성
	{
		DeletePopup = WidgetTree->ConstructWidget<UPopUp>(PopupClass, TEXT("PopUp"));
		RootCanvas->AddChild(DeletePopup);
		DeletePopup->ConfirmButton->OnPressed.AddDynamic(this, &UInventory::ThrowAway);	// 확인 버튼을 누르면 아이템을 삭제
		DeletePopup->CancleButton->OnPressed.AddDynamic(this, &UInventory::RestoreButtons);	// 취소 버튼을 누르면 팝업창만 삭제
	}
	else
	{
		RootCanvas->AddChild(DeletePopup);
	}

	FAnchors anchor;
	anchor.Maximum = FVector2D(0.5f, 0.5f);
	anchor.Minimum = FVector2D(0.5f, 0.5f);
	Cast<UCanvasPanelSlot>(DeletePopup->Slot)->SetAnchors(anchor);
	// Delete Popup의 버릴 개수를 선택된 아이템의 최대 개수로 변경
	DeletePopup->InitializeNumber(Player->Inventory[(uint8)CapturedInfo.ItemType].ItemForms[CapturedInfo.index].ShortForm.Num);

	DisableButtonsTemporarily();	// 현재 버튼들을 임시로 비활성화
}

void UInventory::OnHovered_GetInfo(int index, ETypeTag type)
{
	TArray<FItemForm>* Currslot = &Player->Inventory[(uint8)type].ItemForms;
	if (index < Currslot->Num())
	{
		InfoText->SetText(FText::FromString((*Currslot)[index].ShortForm.InfoTag));	// 아이템의 정보를 Info Text에 출력
	}
}

void UInventory::OnUnhovered_DelInfo()
{
	InfoText->SetText(FText::FromString(""));	// Info Text를 비움
}

void UInventory::OnClicked_Select(int index, ETypeTag type)
{
	TArray<FItemForm>* Currslot = &Player->Inventory[(uint8)type].ItemForms;
	if (index < Currslot->Num())
	{
		Player->Quickslots_Now[(uint8)type] = index;	// 해당 아이템을 착용
		Player->RefreshInventory(type);
		RefreshSlots();

		if (type == ETypeTag::Weapon)
		{
			Player->unEquip();
			Player->Equip();
		}
		if (type != ETypeTag::Item)
		{
			Preview->Sync_to_Character();	// 선택한 아이템이 겉으로 표시되는 경우, Preview Actor를 동기화시켜 표시
		}
	}

	UGameplayStatics::PlaySound2D(this, SelectSound);
}

void UInventory::OnPressed_Catch(int index, ETypeTag type)
{
	// 캡쳐 이미지의 위치를 마우스에 종속
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
		CapturedImage->SetOpacity(0.7f);	// 캡쳐 이미지는 반투명한 상태
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
		if (Pos.X > 1556.0f && Pos.X < 1756.0f && Pos.Y > 420.0f && Pos.Y < 698.0f && CapturedInfo.index > 0)	// 휴지통 위에 캡쳐 이미지가 놓아짐
		{
			OpenPopup();	// Delete 팝업창을 엶
			UGameplayStatics::PlaySound2D(this, PopupSound);
		}
		else
		{
			UGameplayStatics::PlaySound2D(this, CatchSound);
		}
		isCaptured = false;
		CapturedImage->SetOpacity(0.0f);	// 캡쳐 이미지를 숨김
		CapturedImage->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	}
}

void UInventory::OnClicked_Tab(int index, ETypeTag type)
{
	switch (CurrTab)	// 비활성화된 탭의 색을 돌려놓음
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
	
	switch(CurrTab)	// 활성화된 탭의 색을 초록색으로 설정
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

	if(toChange) toChange->SetText(FText::FromString(FString::FromInt(ChangedItem->ShortForm.Num)));	// 바뀐 아이템 개수를 Item Numbers 텍스트 블록에 적용
}

void UInventory::ThrowAway()
{
	if (DeletePopup)
	{
		FItemForm* Selected = &Player->Inventory[(uint8)CapturedInfo.ItemType].ItemForms[CapturedInfo.index];
		Player->ReportItem(Selected->ShortForm.NameTag, -DeletePopup->CurrNum);
		if (DeletePopup->CurrNum < DeletePopup->MaxNum)	// 개수만 줄여주면 됨
		{
			Selected->ShortForm.Num -= DeletePopup->CurrNum;
			ChangeNumber(CapturedInfo.index, CapturedInfo.ItemType);
			RestoreButtons();
		}
		else	// 아이템을 인벤토리에서 삭제해야 함
		{
			Player->DeleteItem(CapturedInfo.ItemType, CapturedInfo.index);
			Preview->Delete_and_Sync(CapturedInfo.ItemType, CapturedInfo.tag);	// 프리뷰 액터에 적용
			RefreshSlots();
		}

		DeletePopup->RemoveFromParent();
	}
}

void UInventory::DisableButtonsTemporarily()
{
	TArray<int> list;

	for (int i = 0; i < Player->Inventory[(uint8)CurrTab].ItemForms.Num(); i++)	// 현재 활성화된 버튼들을 비활성화하고, 임시 리스트에 추가
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
		ItemButtons[idx]->SetIsEnabled(true);	// DisableButtonsTemporarily() 에서 비활성화시킨 버튼들을 다시 활성화함
	}
}