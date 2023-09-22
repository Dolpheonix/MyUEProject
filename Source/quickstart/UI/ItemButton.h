// UI - Item Button
// 인덱스와 아이템 타입을 갖고있는 버튼
// Hovered, Clicked, Pressed 시에 index와 type을 변수로 갖는 이벤트 호출
#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ItemButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemButtonEvent, int, index, ETypeTag, type);

UCLASS()
class QUICKSTART_API UItemButton : public UButton
{
	GENERATED_BODY()
	
public:
	UItemButton();
	UItemButton(int i);

	// 버튼 이벤트를 바인딩
	void SetEvent();
	
	UFUNCTION()
	void HoverTrigger();
	UFUNCTION()
	void ClickTrigger();
	UFUNCTION()
	void PressTrigger();

	FItemButtonEvent HoveredEvent;
	FItemButtonEvent ClickedEvent;
	FItemButtonEvent PressedEvent;

public:
	// 아이템의 인덱스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button")
	int Index;
	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	ETypeTag Type;
};
