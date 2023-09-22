// UI - Notify
// 알림 팝업창
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Utils/NPCUtil.h"
#include "Notify.generated.h"

UCLASS()
class QUICKSTART_API UNotify : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	// 알림창의 텍스트를 변경
	void SetNotification(FString str);
	// 확인 버튼을 누르면, 팝업창을 삭제
	UFUNCTION()
	virtual void OnPressed_Confirm();

public:
	// 확인 버튼
	UPROPERTY(BlueprintReadWrite)
	UButton* ConfirmButton;
	// 알림 텍스트
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* NotifyText;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;
};

// 퀘스트 리워드를 받은 경우에 사용할 알림 팝업창
UCLASS()
class QUICKSTART_API UNotifyAcquire : public UNotify
{
	GENERATED_BODY()
	
public:
	// 표시할 리워드 내용에 맞게 Notification 배열 초기화
	void InitializeNotifications(TArray<FReward> notifications);
	// 다음 알림 내용을 표시
	void SetNotification();
	// 확인 버튼을 누르면, 다음 알림을 표시하거나 삭제
	virtual void OnPressed_Confirm() override;


public:
	// 표시할 리워드들
	TArray<FReward> Notifications;
	// 현재 표시중인 리워드 인덱스
	int currIndex = -1;
};
