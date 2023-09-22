// UI - Quest Table
// 플레이어가 진행중인 퀘스트 정보 표시
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/Overlay.h"
#include "QuestTable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestButtonEvent, int, index);

// 퀘스트 슬롯 버튼
// 인덱스와, 인덱스를 파라미터로 가지는 클릭 이벤트 구현
UCLASS()
class QUICKSTART_API UQuestButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	// 클릭 이벤트
	FQuestButtonEvent ClickedEvent;

	UFUNCTION()
	void FireClickEvent();

public:
	// 슬롯 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* QuestSlotButton;
	// 퀘스트 이름을 표시할 텍스트 블록
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* QuestSlotNameText;
	// 퀘스트의 인덱스 (플레이어에게 등록된 순서)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Index")
	int Index;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;
};

UCLASS()
class QUICKSTART_API UQuestTable : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	// 퀘스트 UI를 엶
	void InitQuestUI(AActor* Instigator);
	// 퀘스트 슬롯을 새로고침
	void RefreshBlocks();
	// 선택된 퀘스트의 정보를 불러옴
	UFUNCTION()
	void OpenQuestInfo(int index);
	// 서브퀘스트 정보를 출력
	FString GetSubQuestText(FSingleQuest* subquest);

public:
	AMainCharacter* Player;
	APlayerController* Controller;

	// 퀘스트 슬롯을 모아놓는 스크롤 박스
	UScrollBox* QuestSlotsScroll;
	// 퀘스트 슬롯 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UQuestButton*> QuestSlotButtons;
	// 퀘스트 정보를 담아놓은 수직 패널
	UVerticalBox* QuestInfoBox;
	// 퀘스트 이름 텍스트
	UTextBlock* QuestNameText;
	// 퀘스트 정보 텍스트
	UTextBlock* QuestInfoText;
	// 서브퀘스트 정보 텍스트
	UTextBlock* SubQuestsText;
	
	// 선택된 슬롯의 인덱스
	int SelectedSlotIndex = -1;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;
};
