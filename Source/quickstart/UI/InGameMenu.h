// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
public:
	UFUNCTION()
	void ReturnToGame();
	UFUNCTION()
	void SaveGame();
	UFUNCTION()
	void ReturnToIntro();

public:
	UButton* BacktoGameButton;
	UButton* SaveGameButton;
	UButton* IntroButton;

	bool bBounded = false;
};
