// UI - Shop
// NPC�� ���� UI. ����, �Ǹ� ��� ����
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

	// �̺�Ʈ ���ε�
	void SetEvents();
	// UI�� ��
	void InitShop(ANPC* Interacted);
	// ���� ������ �ʱ�ȭ
	void InitShopSlots();
	// ĳ���� ������ �ʱ�ȭ
	void InitCharacterSlots();

	// ĳ���� ������ ���� ��ȯ
	UFUNCTION()
	void TabCharacterSlot(int index, ETypeTag type);
	// ������ ���� ����
	UFUNCTION()
	void Increase();
	// ������ ���� ����
	UFUNCTION()
	void Decrease();
	// �� �������� ����
	UFUNCTION()
	void SelectBuyItem(int index, ETypeTag type);
	// �� �������� ����
	UFUNCTION()
	void SelectSellItem(int index, ETypeTag type);
	// ����/�ǸŸ� ����
	UFUNCTION()
	void Deal();
	// ������ ����
	UFUNCTION()
	void EndShop();
	// ȿ���� ���
	UFUNCTION()
	void PlaySelectSound();

	// ������ �����
	void Unselect();
	// ����/���� ��ư�� Ȱ��ȭ ���θ� ����
	void RefreshIncDec();
	// ���� ��, ���� ������ ���ΰ�ħ
	void RefreshShopSlots(int changed);
	// ����/�Ǹ� ��, ĳ���� ������ ���ΰ�ħ
	void RefreshCharacterSlots(ETypeTag type, int changed);
	// ������ ���� ǥ�ø� ����
	void ChangeNumber(bool isShopslot, int index);

public:
	// ���� ���� �迭
	TArray<UItemButton*> ShopSlots;
	// ĳ���� ���� �迭
	TArray<UItemButton*> CharacterSlots;
	// ĳ���� ������ ��
	TArray<UItemButton*> TabSlots;
	// ���� ������ ������ ������ ǥ��
	TArray<UTextBlock*> ShopSlotNumbers;
	// ĳ���� ������ ������ ������ ǥ��
	TArray<UTextBlock*> CharacterSlotNumbers;

	// ���� ��ư
	UButton* DecreaseButton;
	// ���� ��ư
	UButton* IncreaseButton;
	// Ȯ�� ��ư
	UButton* ConfirmationButton;
	// ���� ��ư
	UButton* EndButton;

	// ���� �̸� �ؽ�Ʈ
	UTextBlock* ShopNameText;
	// ���� ���� �ؽ�Ʈ
	UTextBlock* NumText;
	// ����/�Ǹ� ǥ�� �ؽ�Ʈ
	UTextBlock* BuySellText;
	// ���� �ؽ�Ʈ
	UTextBlock* PriceText;
	// ĳ���� Money �ؽ�Ʈ
	UTextBlock* WalletText;

	// ȿ����
	USoundCue* BuySellSound;
	USoundCue* SelectSound;
	USoundCue* IncreaseSound;
	USoundCue* DecreaseSound;
	USoundCue* ExitSound;

	// ������ ������ �⺻ �̹���
	UTexture2D* DefaultTex;
	FSlateBrush DefaultBrush;
	FButtonStyle DefaultStyle;

	// �⺻ ��Ʈ
	FSlateFontInfo DefaultFont;

	// ��ȣ�ۿ� ���� NPC
	ANPC* InteractedNPC;

	AMainCharacter* MainCharacter;
	APlayerController* Controller;

	// ĳ���� ������ ���� ��
	ETypeTag CurrTab;
	// ������ �������� ������
	FItemForm* SelectedItem;
	// ���� ������ ����
	int CurrNum;
	// ����/�Ǹ� ����ΰ�?
	bool bIsBuying;
	// ���� ���õ� �������� �ε��� 
	int CurrIndex;
	// ����/�Ǹ��� �������� ����
	int CurrPrice;

	// PreConstruct()�� ó�� �ҷ��� ���� ������ �ʱ�ȭ�ϱ� ���� ���
	bool Bounded = false;
};
