// UI - Main Widget
// ���� �÷��� �߿� ǥ�õǴ� �⺻ UI
// HP �ٿ� ������ ������, �˸� �˾�â
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
	// ���� HP�� HP �ٿ� ����
	void RefreshHPBar();
	// �������� ��ȯ
	void RefreshQuickslot();
	
public:
	// PreConstruct()�� ó�� �ҷ��� ���� ������ �ʱ�ȭ�ϱ� ���� ���
	bool Bounded = false;

	// HP ��
	UProgressBar* HPBarWidget;
	// ������ �̹���
	TArray<UImage*> QuickslotImage_Weapon;
	// ������ �̹���
	TArray<UImage*> QuickslotImage_Item;
	// �˸�â �̹���
	UImage* NotifyInteractImage;
	// �˸�â�� �ؽ�Ʈ
	UTextBlock* NotifyInteractTextBlock;

};
