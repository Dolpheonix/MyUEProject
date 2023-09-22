// UI - Shop
// NPC의 상점 UI. 구매, 판매 기능 구현
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Textblock.h"
#include "Components/CanvasPanel.h"
#include "ItemButton.h"
#include "../Character/NPC/NPC.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PlayerController.h"
#include "Shop.generated.h"

UCLASS()
class QUICKSTART_API UShop : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 이벤트 바인딩
	void SetEvents();
	// UI를 엶
	void InitShop(ANPC* Interacted);
	// 상점 슬롯을 초기화
	void InitShopSlots();
	// 캐릭터 슬롯을 초기화
	void InitCharacterSlots();

	// 캐릭터 슬롯의 탭을 전환
	UFUNCTION()
	void TabCharacterSlot(int index, ETypeTag type);
	// 아이템 개수 증가
	UFUNCTION()
	void Increase();
	// 아이템 개수 감소
	UFUNCTION()
	void Decrease();
	// 살 아이템을 선택
	UFUNCTION()
	void SelectBuyItem(int index, ETypeTag type);
	// 팔 아이템을 선택
	UFUNCTION()
	void SelectSellItem(int index, ETypeTag type);
	// 구매/판매를 진행
	UFUNCTION()
	void Deal();
	// 상점을 닫음
	UFUNCTION()
	void EndShop();
	// 효과음 재생
	UFUNCTION()
	void PlaySelectSound();

	// 선택을 취소함
	void Unselect();
	// 증가/감소 버튼의 활성화 여부를 결정
	void RefreshIncDec();
	// 구매 시, 상점 슬롯을 새로고침
	void RefreshShopSlots(int changed);
	// 구매/판매 시, 캐릭터 슬롯을 새로고침
	void RefreshCharacterSlots(ETypeTag type, int changed);
	// 아이템 개수 표시를 변경
	void ChangeNumber(bool isShopslot, int index);

public:
	// 상점 슬롯 배열
	TArray<UItemButton*> ShopSlots;
	// 캐릭터 슬롯 배열
	TArray<UItemButton*> CharacterSlots;
	// 캐릭터 슬롯의 탭
	TArray<UItemButton*> TabSlots;
	// 상점 슬롯의 아이템 개수를 표시
	TArray<UTextBlock*> ShopSlotNumbers;
	// 캐릭터 슬롯의 아이템 개수를 표시
	TArray<UTextBlock*> CharacterSlotNumbers;

	// 감소 버튼
	UButton* DecreaseButton;
	// 증가 버튼
	UButton* IncreaseButton;
	// 확인 버튼
	UButton* ConfirmationButton;
	// 종료 버튼
	UButton* EndButton;

	// 상점 이름 텍스트
	UTextBlock* ShopNameText;
	// 선택 개수 텍스트
	UTextBlock* NumText;
	// 구매/판매 표시 텍스트
	UTextBlock* BuySellText;
	// 가격 텍스트
	UTextBlock* PriceText;
	// 캐릭터 Money 텍스트
	UTextBlock* WalletText;

	// 효과음
	USoundCue* BuySellSound;
	USoundCue* SelectSound;
	USoundCue* IncreaseSound;
	USoundCue* DecreaseSound;
	USoundCue* ExitSound;

	// 아이템 슬롯의 기본 이미지
	UTexture2D* DefaultTex;
	FSlateBrush DefaultBrush;
	FButtonStyle DefaultStyle;

	// 기본 폰트
	FSlateFontInfo DefaultFont;

	// 상호작용 중인 NPC
	ANPC* InteractedNPC;

	AMainCharacter* MainCharacter;
	APlayerController* Controller;

	// 캐릭터 슬롯의 현재 탭
	ETypeTag CurrTab;
	// 선택한 아이템의 포인터
	FItemForm* SelectedItem;
	// 현재 설정된 개수
	int CurrNum;
	// 구매/판매 모드인가?
	bool bIsBuying;
	// 현재 선택된 아이템의 인덱스 
	int CurrIndex;
	// 구매/판매할 아이템의 가격
	int CurrPrice;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;
};
