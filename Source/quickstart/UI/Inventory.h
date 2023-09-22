// UI - Inventory
// 플레이어의 인벤토리 UI
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ItemButton.h"
#include "../Character/Main/MainCharacter.h"
#include "PopUp.h"
#include "PreviewActor.h"
#include "Sound/SoundCue.h"
#include "Inventory.generated.h"

UCLASS()
class QUICKSTART_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& Mygeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	// 아이템 슬롯을 바인드
	void InitializeSlots();
	// 현재 인벤토리에 맞게 슬롯 업데이트
	void RefreshSlots();
	// 버튼 이벤트를 바인드
	void SetEvents();
	// 팝업 창을 엶
	void OpenPopup();
	// 팝업 창이 열렸을 때, 기존 버튼들을 비활성화
	UFUNCTION()
	void DisableButtonsTemporarily();
	// 다시 버튼들을 활성화함
	UFUNCTION()
	void RestoreButtons();
	
	// 마우스가 아이템 슬롯 위에 있을 때, 아이템의 정보를 표시
	UFUNCTION()
	void OnHovered_GetInfo(int index, ETypeTag type);
	// 아이템 정보 표시를 삭제
	UFUNCTION()
	void OnUnhovered_DelInfo();
	// 슬롯을 클릭했을 때, 해당 아이템을 선택
	UFUNCTION()
	void OnClicked_Select(int index, ETypeTag type);
	// 슬롯을 눌렀을 때, 해당 슬롯을 캡쳐 (드래그)
	UFUNCTION()
	void OnPressed_Catch(int index, ETypeTag type);
	// 캡쳐된 슬롯을 휴지통에서 Release했을 때, 해당 아이템을 삭제
	UFUNCTION()
	void OnReleased_Delete();
	// 아이템 탭을 클릭했을 때, 해당 탭으로 슬롯 전환
	UFUNCTION()
	void OnClicked_Tab(int index, ETypeTag type);
	// 아이템을 삭제
	UFUNCTION()
	void ThrowAway();
	// 인벤토리를 나감
	UFUNCTION()
	void Exit();
	// 
	void ChangeNumber(int index, ETypeTag type);


public:
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* RootCanvas = nullptr;
	// 아이템 정보를 출력하는 텍스트 블록
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* InfoText;
	// 아이템 슬롯
	TArray<UItemButton*> ItemButtons;
	// 아이템 개수
	TArray<UTextBlock*> ItemNumbers;
	// 인벤토리의 탭
	TArray<UItemButton*> ItemTabs;
	// 탭의 텍스트 블록
	TArray<UTextBlock*> TabTexts;

	// 현재 선택된 탭
	ETypeTag CurrTab;
	UTextBlock* CurrTabText;

	// 현재 보유한 Money를 출력하는 텍스트 불록
	UTextBlock* CurrMoneyText;

	// 휴지통 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* TrashCan;
	// Exit 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* ExitButton;
	// 팝업 UI의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PopUp")
	TSubclassOf<UPopUp> PopupClass;
	// 아이템을 휴지통에 버릴 때, 확인용 팝업창
	UPopUp* DeletePopup;

	// 아이템 선택 시 재생되는 효과음
	USoundCue* SelectSound;
	// 아이템을 캡쳐할 때 재생되는 효과음
	USoundCue* CatchSound;
	// 팝업 창이 열릴 때 재생되는 효과음
	USoundCue* PopupSound;
	// UI를 나갈 때 재생되는 효과음
	USoundCue* ExitSound;

	// 아이템 슬롯의 기본 이미지
	UTexture2D* DefaultTex;
	FSlateBrush DefaultBrush;
	FButtonStyle DefaultStyle;
	// 기본 폰트
	FSlateFontInfo DefaultFont;

	// 플레이어
	UPROPERTY(BlueprintReadOnly)
	AMainCharacter* Player;

	UPROPERTY(BlueprintReadOnly)
	APlayerController* Controller;

	// 프리뷰 액터
	APreviewActor* Preview;

	// 슬롯 캡쳐 시 반투명하게 나타나는 슬롯 이미지
	UPROPERTY(BlueprintReadWrite)
	UImage* CapturedImage;
	// 현재 슬롯을 캡쳐중인가?
	UPROPERTY(BlueprintReadWrite)
	bool isCaptured = false;
	// 캡쳐된 슬롯의 정보
	CapturedSlot_Info CapturedInfo;

	// 팝업 창이 열렸을 때, 비활성화된 버튼의 목록
	TArray<int> ShutdownList;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;
};
