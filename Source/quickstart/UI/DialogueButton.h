// UI - Dialogue Button
// 버튼을 눌렀을 때 index를 Parameter로 갖는 이벤트 호출
#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "DialogueButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueButtonEvent, int, index);

UCLASS()
class QUICKSTART_API UDialogueButton : public UButton
{
	GENERATED_BODY()
	
public:
	UDialogueButton();
	UDialogueButton(int i);

	UFUNCTION()
	void ClickTrigger();

	FDialogueButtonEvent DialogueButtonEvent;

public:
	// 버튼의 인덱스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	int index;
};
