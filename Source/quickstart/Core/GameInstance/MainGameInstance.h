// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../../Utils/Structs.h"
#include "../../Utils/NPCUtil.h"
#include "../../Character/Main/MainCharacter.h"
#include "../SaveObject/GameSaver.h"
#include "MainGameInstance.generated.h"

UCLASS()
class QUICKSTART_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMainGameInstance();
	virtual void Init() override;
	//virtual void StartGameInstance() override;
	virtual void Shutdown() override;

public:
	void InitializeCharacterMemory(FString Name);
	void ApplyCharacterMemory(AMainCharacter* character);
	void SaveCharacterMemory(AMainCharacter* character);

	bool SaveToFile();
	TArray<FQuestStatus> SaveQuestStatus();
	void SaveLevel();
	void SaveNPC();
	void SaveItems();

	void LoadFromFile(FString Slotname);
	void LoadQuestStatus(TArray<FQuestStatus> QuestStatus);
	UFUNCTION(BlueprintCallable)
	void LoadLevel(FString MapName, bool isStart);
	UFUNCTION()
	void LoadNPC(UWorld* world);
	UFUNCTION()
	void LoadItems(UWorld* world);

	void Start();

	int32 AllocateSlotIndex();

	FQuest* GetQuest(int index);

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FQuest> Quests;

	UPROPERTY(BlueprintReadWrite)
	FCharacterMemory CharacterMemory;
	UPROPERTY(BlueprintReadWrite)
	FWorkingQuestMemory CharacterMemory_Quest;
	UPROPERTY(BlueprintReadWrite)
	TArray<FMapMemory> MapMemories;

	FMapMemory* CurrentMapMemory;
	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex;

	bool bApplyBound = false;
	bool bToInitializeMapMemory = true;
};

/* TODO
	1. LoadCharacterMemory 시에 inventory item 썸네일, 매시 임포트
	2. Load... 추가
	3. 
*/