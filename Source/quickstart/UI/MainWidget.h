// UI - Main Widget
// 게임 플레이 중에 표시되는 기본 UI
// HP 바와 아이템 퀵슬롯, 알림 팝업창
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../Utils/Structs.h"
#include "MainWidget.generated.h"

UCLASS()
class QUICKSTART_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	// 현재 HP를 HP 바에 적용
	void RefreshHPBar();
	// 퀵슬롯을 전환
	void RefreshQuickslot();
	
public:
	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;

	// HP 바
	UProgressBar* HPBarWidget;
	// 퀵슬롯 이미지
	TArray<UImage*> QuickslotImage_Weapon;
	// 퀵슬롯 이미지
	TArray<UImage*> QuickslotImage_Item;
	// 알림창 이미지
	UImage* NotifyInteractImage;
	// 알림창의 텍스트
	UTextBlock* NotifyInteractTextBlock;

};
