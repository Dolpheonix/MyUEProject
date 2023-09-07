// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../Utils/Structs.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& Mygeometry, float InDeltaTime) override;

public:
	void RefreshHPBar();
	void RefreshQuickslot();
	
public:
	bool Bounded = false;

	UProgressBar* HPBarWidget;
	UImage* MinimapImage;
	TArray<UImage*> QuickslotImage_Weapon;
	TArray<UImage*> QuickslotImage_Item;
	UImage* NotifyInteractImage;
	UTextBlock* NotifyInteractTextBlock;

};
