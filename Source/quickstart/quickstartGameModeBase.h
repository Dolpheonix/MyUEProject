// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "quickstartGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API AquickstartGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AquickstartGameModeBase();

	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeMenuWidget(UUserWidget* TargetUI);

	UFUNCTION()
	void SniperMode(bool turnOn);

protected:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> ShowroomWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> SniperWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> DialogueBoxWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> ShopWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> QuestWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* MainUI;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ShowroomUI;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* SniperUI;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* DialogueBoxUI;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ShopUI;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* QuestUI;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* CurrentUI;

	class UDataTable* ItemTable;
	class UDataTable* WeaponTable;
	class UDataTable* ClothTable;

public:
	UPROPERTY(BlueprintReadWrite, Category = "UMG_Game")
	bool bShowroom=false;
};
