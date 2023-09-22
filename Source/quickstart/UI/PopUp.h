// UI - Popup
// 인벤토리 UI에서 아이템 삭제 시, 개수 설정 및 최종 확인용 팝업창
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Sound/SoundCue.h"
#include "PopUp.generated.h"

UCLASS()
class QUICKSTART_API UPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	// 변수 초기화
	void InitializeNumber(int max);
	// CurrNum에 따라 증가/감소 버튼의 활성화 여부를 결정
	void Refresh();
	// 숫자 입력창에 내용을 입력했을 때, 이를 적용
	UFUNCTION()
	void ApplyInput(const FText& Text, ETextCommit::Type CommitMethod);
	// 아이템 개수 증가
	UFUNCTION()
	void Increase();
	// 아이템 개수 감소
	UFUNCTION()
	void Decrease();
	// 확인 버튼 누를 시
	UFUNCTION()
	void Confirm();
	// 취소 버튼 누를 시
	UFUNCTION()
	void Cancle();

public:
	// 증가 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* IncreaseButton;
	// 감소 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* DecreaseButton;
	// 확인 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* ConfirmButton;
	// 취소 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* CancleButton;
	// 숫자 입력창
	UPROPERTY(BlueprintReadWrite)
	UEditableTextBox* NumberInput;
	
	// 효과음
	USoundCue* IncreaseSound;
	USoundCue* DecreaseSound;
	USoundCue* ConfirmSound;
	USoundCue* CancleSound;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;

	// 현재 설정된 Num
	int CurrNum = -1;
	// 최대로 선택할 수 있는 Num (삭제할 아이템의 보유 개수)
	int MaxNum = -1;
};