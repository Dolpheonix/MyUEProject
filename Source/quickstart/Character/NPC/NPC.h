// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character_Root.h"
#include "../Main/MainCharacter.h"
#include "../../Interface/InteractionInterface.h"
#include "../../Utils/Structs.h"
#include "../../Utils/NPCUtil.h"
#include "AIController.h"
#include "NPC.generated.h"
/**
 * 
 */
UCLASS()
class QUICKSTART_API ANPC : public ACharacter_Root, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	ANPC();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& e) override;

	virtual void OnHurt() override;
	virtual void OnDead() override;

	virtual void Interact() override;
	virtual void UnInteract() override;

	void OpenShop();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* SelfCamera;
	
	UPROPERTY(BlueprintReadWrite)
	AAIController* AIController;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FDialogueTree DialogueTree;

	UPROPERTY(BlueprintReadWrite)
	AMainCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FShopItemShortForm> ShopItemsInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acquisition")
	TArray<FItemShortForm> AcquireItemsInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FQuest> Quests;

	TArray<FShopItemForm> ShopItems;
	TArray<FItemForm> AcquireItems;

	FVector InteractionPoint;

	bool bInteractable = false;
	bool bInteracted = false;

	class UDataTable* ItemTable;
	class UDataTable* WeaponTable;
	class UDataTable* ClothTable;
};
