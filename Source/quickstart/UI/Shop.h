// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Textblock.h"
#include "ItemButton.h"
#include "../Character/NPC/NPC.h"
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
	//UFUNCTION() // 아이템 갯수 increase
	//void Increase();
	//UFUNCTION() // 아이템 갯수 decrease
	//void Decrease();
	//UFUNCTION() // 살 아이템을 선택(Click)
	//void SelectBuyItem(int index, ETypeTag type);
	//UFUNCTION() // 팔 아이템을 선택(Click)
	//void SelectSellItem(int index, ETypeTag type);
	UFUNCTION()
	void EndShop();

public:
	TArray<UItemButton*> ShopSlots;
	TArray<UItemButton*> CharacterSlots;
	TArray<UItemButton*> TabSlots;

	UButton* DecreaseButton;
	UButton* IncreaseButton;
	UTextBlock* NumText;
	UTextBlock* BuySellText;

	UButton* EndButton;

	ANPC* InteractedNPC;

	AMainCharacter* MainCharacter;
	APlayerController* Controller;

	ETypeTag CurrTab;
	FItemForm SelectedItem;
	bool bIsBuying;

	bool Bounded = false;
};
