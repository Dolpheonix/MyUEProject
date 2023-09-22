// UI - Intro
// 인트로 UI
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Intro.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadSlotClickedEvent, FString, Name);

UCLASS()
class QUICKSTART_API USlotButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	// 슬롯의 이름이 Parameter인 이벤트
	FLoadSlotClickedEvent ClickedEvent;

	UFUNCTION()
	void FireClickEvent();

public:
	// 눌렀을 때, 세이브 파일 정보를 로드하는 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* GetInfoButton;
	// 세이브 슬롯의 이름을 표시하는 텍스트
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* SlotNameText;
	// 슬롯의 인덱스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Index")
	int Index = -1;
	// 슬롯의 이름
	FString Name;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;
};

UCLASS()
class QUICKSTART_API UIntro : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// New Game UI를 엶
	UFUNCTION()
	void OpenNewGameUI();
	// Load Game UI를 엶
	UFUNCTION()
	void OpenLoadGameUI();
	// Setting UI를 엶
	UFUNCTION()
	void OpenSettingUI();
	// 게임을 종료
	UFUNCTION()
	void Exit();
	// 새 게임을 시작함
	UFUNCTION()
	void StartNewGame();
	// 선택한 게임을 불러옴
	UFUNCTION()
	void StartLoadGame();
	// 슬롯의 정보를 가져옴
	UFUNCTION()
	void GetSlotInfo(FString Name);

public:
	// 슬롯 버튼 UI의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubWidget")
	TSubclassOf<USlotButton> SlotButtonWidgetClass;

	// 기본 인트로 UI
	// 캔버스
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* Intro_Canvas;
	// New Game 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* Intro_NewGameButton;
	// Load Game 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* Intro_LoadGameButton;
	// Setting 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* Intro_SettingButton;
	// Exit 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* Intro_ExitButton;

	// New Game UI
	// 캔버스
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* NewGame_Canvas;
	// 새로 생성할 캐릭터 이름
	UPROPERTY(BlueprintReadWrite)
	UEditableText* NewGame_CharacterNameText;	
	// 새 게임을 시작하는 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* NewGame_GameStartButton;

	// Load Game UI
	// 캔버스
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* LoadGame_Canvas;
	// 배경 이미지
	UPROPERTY(BlueprintReadWrite)
	UImage* LoadGame_InfoBackgroundImage;
	// 슬롯을 모아놓은 스크롤 박스
	UPROPERTY(BlueprintReadWrite)
	UScrollBox* LoadGame_SaveSlotsScrollBox;
	// 슬롯 버튼
	UPROPERTY(BlueprintReadWrite)
	TArray<USlotButton*> LoadGame_SlotButtons;
	// 선택된 세이브 파일의 이름을 표시할 UI
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* LoadGame_InfoNameText;
	// 선택된 세이브 파일의 정보를 표시할 UI
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* LoadGame_InfoText;
	// 선택된 세이브 파일을 불러오는 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* LoadGame_GameStartButton;
	// 선택된 세이브 파일의 이름
	FString LoadGame_SelectedName;
	// 선택된 세이브 파일의 인덱스
	int LoadGame_SelectedIndex;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;
};
