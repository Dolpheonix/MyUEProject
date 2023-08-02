// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Intro.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadSlotClickedEvent, FString, Name);

UCLASS()
class QUICKSTART_API USlotButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	FLoadSlotClickedEvent ClickedEvent;

	UFUNCTION()
	void FireClickEvent();

public:
	UPROPERTY(BlueprintReadWrite)
	UButton* GetInfoButton;

	UPROPERTY(BlueprintReadWrite)
	UTextBlock* SlotNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Index")
	int Index = -1;

	FString Name;

	bool Bounded = false;
};

UCLASS()
class QUICKSTART_API UIntro : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OpenNewGameUI();
	UFUNCTION()
	void OpenLoadGameUI();
	UFUNCTION()
	void OpenSettingUI();
	UFUNCTION()
	void Exit();
	UFUNCTION()
	void StartNewGame();
	UFUNCTION()
	void StartLoadGame();
	UFUNCTION()
	void GetSlotInfo(FString Name);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SubWidget")
	TSubclassOf<USlotButton> SlotButtonWidgetClass;

	// Intro
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* Intro_Canvas;
	UPROPERTY(BlueprintReadWrite)
	UButton* Intro_NewGameButton;
	UPROPERTY(BlueprintReadWrite)
	UButton* Intro_LoadGameButton;
	UPROPERTY(BlueprintReadWrite)
	UButton* Intro_SettingButton;
	UPROPERTY(BlueprintReadWrite)
	UButton* Intro_ExitButton;

	// New Game
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* NewGame_Canvas;
	UPROPERTY(BlueprintReadWrite)
	UEditableText* NewGame_CharacterNameText;	
	UPROPERTY(BlueprintReadWrite)
	UButton* NewGame_GameStartButton;

	// Load Game
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* LoadGame_Canvas;
	UPROPERTY(BlueprintReadWrite)
	UImage* LoadGame_InfoBackgroundImage;
	UPROPERTY(BlueprintReadWrite)
	UScrollBox* LoadGame_SaveSlotsScrollBox;
	UPROPERTY(BlueprintReadWrite)
	TArray<USlotButton*> LoadGame_SlotButtons;
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* LoadGame_InfoNameText;
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* LoadGame_InfoText;
	UPROPERTY(BlueprintReadWrite)
	UButton* LoadGame_GameStartButton;
	
	FString LoadGame_SelectedName;
	int LoadGame_SelectedIndex;

	// Setting
	UPROPERTY(BlueprintReadWrite)
	UButton* Setting_ResolutionUpButton;
	UPROPERTY(BlueprintReadWrite)
	UButton* Setting_ResolutionDownButton;
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* Setting_ResolutionText;

	bool Bounded = false;
};
