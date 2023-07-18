// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "../Utils/NPCUtil.h"
#include "Notify.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API UNotify : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	void SetNotification(FString str);
	UFUNCTION()
	virtual void OnPressed_Confirm();

public:
	UPROPERTY(BlueprintReadWrite)
	UButton* ConfirmButton;

	UPROPERTY(BlueprintReadWrite)
	URichTextBlock* NotifyText;

	bool Bounded = false;
};

UCLASS()
class QUICKSTART_API UNotifyAcquire : public UNotify
{
	GENERATED_BODY()
	
public:

	void InitializeNotifications(TArray<FReward> notifications);

	void SetNotification();

	virtual void OnPressed_Confirm() override;


public:
	TArray<FReward> Notifications;

	int currIndex = -1;
};
