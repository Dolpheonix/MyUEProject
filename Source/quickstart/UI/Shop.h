// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
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

	void InitShop(ANPC* Interacted);

	UFUNCTION()
	void EndShop();

public:
	UButton* EndButton;

	ANPC* InteractedNPC;

	APlayerController* Controller;

	bool Bounded = false;
};
