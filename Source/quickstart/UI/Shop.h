// Fill out your copyright notice in the Description page of Project Settings.

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

/**
 * 
 */
UCLASS()
class QUICKSTART_API UShop : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetEvents();

	void InitShop(ANPC* Interacted);
	void InitShopSlots();
	void InitCharacterSlots();

	UFUNCTION()
	void TabCharacterSlot(int index, ETypeTag type);
	UFUNCTION() // 아이템 갯수 increase
	void Increase();
	UFUNCTION() // 아이템 갯수 decrease
	void Decrease();
	UFUNCTION() // 살 아이템을 선택(Click)
	void SelectBuyItem(int index, ETypeTag type);
	UFUNCTION() // 팔 아이템을 선택(Click)
	void SelectSellItem(int index, ETypeTag type);
	UFUNCTION()
	void Deal();
	UFUNCTION()
	void EndShop();
	UFUNCTION()
	void PlaySelectSound();

	void Unselect();
	void RefreshIncDec();
	void RefreshShopSlots(int changed);
	void RefreshCharacterSlots(ETypeTag type, int changed);
	void ChangeNumber(bool isShopslot, int index);

public:
	UCanvasPanel* RootCanvas;

	TArray<UItemButton*> ShopSlots;
	TArray<UItemButton*> CharacterSlots;
	TArray<UItemButton*> TabSlots;

	TArray<UTextBlock*> ShopSlotNumbers;
	TArray<UTextBlock*> CharacterSlotNumbers;

	UButton* DecreaseButton;
	UButton* IncreaseButton;
	UButton* ConfirmationButton;
	UButton* EndButton;

	UTextBlock* NumText;
	UTextBlock* BuySellText;
	UTextBlock* PriceText;
	UTextBlock* WalletText;

	USoundCue* BuySellSound;
	USoundCue* SelectSound;
	USoundCue* IncreaseSound;
	USoundCue* DecreaseSound;
	USoundCue* ExitSound;

	UTexture2D* DefaultTex;
	FSlateBrush DefaultBrush;
	FButtonStyle DefaultStyle;

	FSlateFontInfo DefaultFont;

	ANPC* InteractedNPC;

	AMainCharacter* MainCharacter;
	APlayerController* Controller;

	// Current State
	ETypeTag CurrTab;
	FItemForm* SelectedItem;
	int CurrNum;
	bool bIsBuying;
	int CurrIndex;
	int CurrPrice;

	bool Bounded = false;
};
