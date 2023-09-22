// UI - Item Button
// �ε����� ������ Ÿ���� �����ִ� ��ư
// Hovered, Clicked, Pressed �ÿ� index�� type�� ������ ���� �̺�Ʈ ȣ��
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

	// ��ư �̺�Ʈ�� ���ε�
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
	// �������� �ε���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button")
	int Index;
	// ������ Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	ETypeTag Type;
};
