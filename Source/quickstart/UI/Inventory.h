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
#include "PopUp.h"
#include "PreviewActor.h"
#include "Sound/SoundCue.h"
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
	void OpenPopup();

	UFUNCTION()
	void DisableButtonsTemporarily();
	UFUNCTION()
	void RestoreButtons();

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

	UFUNCTION()
	void ThrowAway();

	void ChangeNumber(int index, ETypeTag type);

public:
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* RootCanvas = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* InfoText;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* WeaponText;
	UPROPERTY(BlueprintReadWrite)
	TArray<UItemButton*> WeaponSlots;
	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> WeaponSlotNumbers;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* ItemText;
	UPROPERTY(BlueprintReadWrite)
	TArray<UItemButton*> ItemSlots;
	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> ItemSlotNumbers;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* ClothText;
	UPROPERTY(BlueprintReadWrite)
	TArray<UItemButton*> ClothSlots;
	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> ClothSlotNumbers;

	UPROPERTY(BlueprintReadWrite)
	UButton* TrashCan;

	UClass* PopupClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PopUp")
	UPopUp* DeletePopup;

	USoundCue* SelectSound;
	USoundCue* CatchSound;
	USoundCue* PopupSound;
	USoundCue* ExitSound;

	UTexture2D* DefaultTex;
	FSlateBrush DefaultBrush;
	FButtonStyle DefaultStyle;

	FSlateFontInfo DefaultFont;

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
	TArray<int> ShutdownList;

	bool Bounded = false;
};
