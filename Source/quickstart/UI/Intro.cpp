﻿#include "Intro.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Notify.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "../quickstart.h"
#include "../Core/GameMode/IntroGameMode.h"
#include "../Core/GameInstance/MainGameInstance.h"
#include "../Core/SaveObject/GameSaver.h"
#include "../Utils/JsonHelpers.h"

void USlotButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	bIsFocusable = true;

	if (!Bounded)
	{
		GetInfoButton = Cast<UButton>(GetWidgetFromName(TEXT("GetInfo")));
		SlotNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SlotName")));

		if (GetInfoButton) GetInfoButton->OnClicked.AddDynamic(this, &USlotButton::FireClickEvent);

		Bounded = true;
	}
}

void USlotButton::NativeConstruct()
{
	Super::NativeConstruct();

	check(GetInfoButton);
	check(SlotNameText);
}

void USlotButton::FireClickEvent()
{
	ClickedEvent.Broadcast(Name);
}

void UIntro::NativePreConstruct()
{
	Super::NativePreConstruct();
	bIsFocusable = true;
	
	if (!Bounded)
	{
		Intro_Canvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Intro")));
		Intro_NewGameButton = Cast<UButton>(GetWidgetFromName(TEXT("Intro_NewGame")));
		Intro_LoadGameButton = Cast<UButton>(GetWidgetFromName(TEXT("Intro_LoadGame")));
		Intro_SettingButton = Cast<UButton>(GetWidgetFromName(TEXT("Intro_Setting")));
		Intro_ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Intro_Exit")));

		NewGame_Canvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("NewGame")));
		NewGame_CharacterNameText = Cast<UEditableText>(GetWidgetFromName(TEXT("NewGame_CharacterName")));
		NewGame_GameStartButton = Cast<UButton>(GetWidgetFromName(TEXT("NewGame_GameStart")));

		LoadGame_Canvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("LoadGame")));
		LoadGame_InfoBackgroundImage = Cast<UImage>(GetWidgetFromName(TEXT("LoadGame_InfoBackground")));
		LoadGame_SaveSlotsScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("LoadGame_SaveSlots")));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_0"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_1"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_2"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_3"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_4"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_5"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_6"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_7"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_8"))));
		LoadGame_SlotButtons.Add(Cast<USlotButton>(GetWidgetFromName(TEXT("LoadGame_SlotButton_9"))));
		LoadGame_InfoNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LoadGame_InfoName")));
		LoadGame_InfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LoadGame_Info")));
		LoadGame_GameStartButton = Cast<UButton>(GetWidgetFromName(TEXT("LoadGame_GameStart")));

		if (Intro_NewGameButton) Intro_NewGameButton->OnClicked.AddDynamic(this, &UIntro::OpenNewGameUI);
		if (Intro_LoadGameButton) Intro_LoadGameButton->OnClicked.AddDynamic(this, &UIntro::OpenLoadGameUI);
		if (Intro_SettingButton) Intro_SettingButton->OnClicked.AddDynamic(this, &UIntro::OpenSettingUI);
		if (Intro_ExitButton) Intro_ExitButton->OnClicked.AddDynamic(this, &UIntro::Exit);
		if (NewGame_GameStartButton) NewGame_GameStartButton->OnClicked.AddDynamic(this, &UIntro::StartNewGame);

		for (int i = 0; i < 10; i++)
		{
			if (LoadGame_SlotButtons[i])
			{
				LoadGame_SlotButtons[i]->Index = i;
				LoadGame_SlotButtons[i]->ClickedEvent.AddDynamic(this, &UIntro::GetSlotInfo);
			}
		}
		if (LoadGame_GameStartButton) LoadGame_GameStartButton->OnClicked.AddDynamic(this, &UIntro::StartLoadGame);

		Bounded = true;
	}
}

void UIntro::NativeConstruct()
{
	Super::NativeConstruct();
	
	check(Intro_NewGameButton);
	check(Intro_LoadGameButton);
	check(Intro_SettingButton);
	check(Intro_ExitButton);

	check(NewGame_CharacterNameText);
	check(NewGame_GameStartButton);

	check(LoadGame_SaveSlotsScrollBox);
	for (int i = 0; i < LoadGame_SlotButtons.Num(); i++)
	{
		check(LoadGame_SlotButtons[i] != nullptr);
	}
	check(LoadGame_InfoNameText);
	check(LoadGame_InfoText);
	check(LoadGame_GameStartButton);

	Intro_Canvas->SetVisibility(ESlateVisibility::Visible);
	NewGame_Canvas->SetVisibility(ESlateVisibility::Collapsed);
	LoadGame_Canvas->SetVisibility(ESlateVisibility::Collapsed);
	LoadGame_SaveSlotsScrollBox->SetVisibility(ESlateVisibility::Collapsed);

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller)
	{
		// 화면 중앙에 마우스 커서 표시
		int X, Y;
		Controller->GetViewportSize(X, Y);
		Controller->SetMouseLocation(X / 2, Y / 2);
		Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
		Controller->SetShowMouseCursor(true);
		Controller->SetInputMode(FInputModeUIOnly());
	}
}

void UIntro::NativeDestruct()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller)
	{
		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
	}

	Super::NativeDestruct();
}

void UIntro::OpenNewGameUI()
{
	// 캔버스 패널 전환
	Intro_Canvas->SetVisibility(ESlateVisibility::Collapsed);
	NewGame_Canvas->SetVisibility(ESlateVisibility::Visible);
}

void UIntro::OpenLoadGameUI()
{
	// 캔버스 패널 전환
	Intro_Canvas->SetVisibility(ESlateVisibility::Collapsed);
	LoadGame_Canvas->SetVisibility(ESlateVisibility::Visible);
	LoadGame_SaveSlotsScrollBox->SetVisibility(ESlateVisibility::Visible);

	TArray<FString> SlotList;
	if (JsonLoader::LoadSlotList(SlotList))	// 세이브 슬롯 목록을 불러옴
	{
		for (int i = 0; i < MAX_SAVE_SLOTS; i++)
		{
			if (SlotList[i] == "")	// 비어있는 슬롯
			{
				LoadGame_SlotButtons[i]->SlotNameText->SetText(FText::FromString("Empty"));
				LoadGame_SlotButtons[i]->SetIsEnabled(false);
			}
			else	// 유효한 슬롯
			{
				LoadGame_SlotButtons[i]->SlotNameText->SetText(FText::FromString(SlotList[i]));
				LoadGame_SlotButtons[i]->Name = SlotList[i];
				LoadGame_SlotButtons[i]->SetIsEnabled(true);
			}
		}
	}
	else	// 불러올 목록이 없음
	{
		for (int i = 0; i < MAX_SAVE_SLOTS; i++)
		{
			LoadGame_SlotButtons[i]->SlotNameText->SetText(FText::FromString(""));
			LoadGame_SlotButtons[i]->SetIsEnabled(false);
		}
	}

	// 세이브 파일 정보창 초기화
	LoadGame_InfoNameText->SetText(FText::FromString("Name : "));
	LoadGame_InfoText->SetText(FText::FromString("Level : \nCurrent Map : "));

	LoadGame_GameStartButton->SetIsEnabled(false);
}

void UIntro::OpenSettingUI()
{

}

void UIntro::Exit()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void UIntro::StartNewGame()
{
	FString Name = NewGame_CharacterNameText->GetText().ToString();	// 입력한 이름

	if (Name.IsEmpty())	// 이름 칸이 비어있을 경우
	{
		AIntroGameMode* IntroGM = Cast<AIntroGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		UNotify* NotifyPopup = WidgetTree->ConstructWidget<UNotify>(IntroGM->NotifyWidgetClass);
		if (NotifyPopup)	// 알림창 팝업
		{
			Cast<UCanvasPanel>(GetRootWidget())->AddChild(NotifyPopup);
			Cast<UCanvasPanelSlot>(NotifyPopup->Slot)->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
			FString str = TEXT("이름을 입력해주세요!");
			NotifyPopup->SetNotification(str);
		}
	}
	else
	{
		auto* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			GI->InitializeCharacterMemory(Name);
			int alloced = GI->AllocateSlotIndex();
			if (alloced < 0)	// 세이브 슬롯을 생성할 수 없는 경우
			{
				AIntroGameMode* IntroGM = Cast<AIntroGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				UNotify* NotifyPopup = WidgetTree->ConstructWidget<UNotify>(IntroGM->NotifyWidgetClass);
				if (NotifyPopup)
				{
					Cast<UCanvasPanel>(GetRootWidget())->AddChild(NotifyPopup);
					Cast<UCanvasPanelSlot>(NotifyPopup->Slot)->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));

					if (alloced == -1)	// 이미 존재하는 이름일 경우
					{
						FString Notification = "Character name " + Name + " already exists";
						NotifyPopup->SetNotification(Notification);
					}
					else	// 슬롯 칸이 꽉찬 경우
					{
						FString Notification = "Character slot is full";
						NotifyPopup->SetNotification(Notification);
					}
				}
			}
			else
			{
				GI->SaveToFile();	// 기본 생성 파일 저장
				GI->Start();	// 게임 시작
			}
		}
	}
}

void UIntro::GetSlotInfo(FString Name)
{
	// 이름에 맞는 세이브파일 로드
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectSavedDir() + "SaveGames/" + Name + ".json"));
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	FJsonSerializer::Deserialize(Reader, JsonObject);

	if (JsonObject.IsValid())
	{
		FCharacterMemory ChMem;
		JsonLoader::LoadCharacterMemory(JsonObject->GetObjectField(TEXT("CharacterMemory")), ChMem);	// 캐릭터 메모리를 불러옴
		
		LoadGame_InfoNameText->SetText(FText::FromString("Name : " + Name));	// 파일의 이름 표시

		FString info = "Level : " + FString::FromInt(ChMem.CurrentLevel);	// 현재 레벨 표시
		info += "\n";
		info += "Current Map : " + ChMem.CurrentMap;	// 현재 위치한 맵 표시
		LoadGame_InfoText->SetText(FText::FromString(info));

		LoadGame_SelectedName = Name;
		LoadGame_GameStartButton->SetIsEnabled(true);
	}
	else	// 파일을 불러올 수 없는 경우
	{
		UE_LOG(LogJson, Error, TEXT("Save File %s not exist"), *Name);
		LoadGame_InfoNameText->SetText(FText::FromString("Name : " + Name));
		LoadGame_InfoText->SetText(FText::FromString("Not Found"));
		LoadGame_GameStartButton->SetIsEnabled(false);
	}
}

void UIntro::StartLoadGame()
{
	auto* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->LoadFromFile(LoadGame_SelectedName);	// 세이브 파일을 불러옴
		GI->Start();	// 불러온 게임을 시작
	}
}