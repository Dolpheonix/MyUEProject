// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ItemButton.h"
#include "../Character/Main/MainCharacter.h"
#include "PreviewActor.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& Mygeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void InitializeSlots();
	void SetTexts();
	void RefreshSlots();
	void SetEvents();

	UFUNCTION()
	void OnHovered_GetInfo(int index, ETypeTag type);
	UFUNCTION()
	void OnUnhovered_DelInfo();
	UFUNCTION()
	void OnClicked_Select(int index, ETypeTag type);
	UFUNCTION()
	void OnPressed_Catch(int index, ETypeTag type);
	UFUNCTION()
	void OnReleased_Delete();

public:
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* MainPanel = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> TextBlocks;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* InfoTextBlock;

	UPROPERTY(BlueprintReadWrite)
	TArray<UItemButton*> WeaponSlots;

	UPROPERTY(BlueprintReadWrite)
	TArray<UItemButton*> ItemSlots;

	UPROPERTY(BlueprintReadWrite)
	TArray<UItemButton*> ClothSlots;

	UPROPERTY(BlueprintReadWrite)
	UButton* TrashCan;

	UPROPERTY(BlueprintReadWrite)
	UTexture2D* defaultSlotTex;

	UPROPERTY(BlueprintReadWrite)
	UObject* TextFont;

	UPROPERTY(BlueprintReadOnly)
	AMainCharacter* Player;

	UPROPERTY(BlueprintReadOnly)
	APlayerController* Controller;

	APreviewActor* Preview;

	UPROPERTY(BlueprintReadWrite)
	UImage* CapturedImage;

	UPROPERTY(BlueprintReadWrite)
	bool isCaptured = false;

	CapturedSlot_Info CapturedInfo;
};
