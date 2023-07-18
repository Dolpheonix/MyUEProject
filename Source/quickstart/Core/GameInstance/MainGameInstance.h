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
	virtual void Shutdown() override;

public:
	void LoadToCharacter(AMainCharacter* character);
	void SaveFromCharacter(AMainCharacter* character);

	void LoadFromFile(FString slotname, int32 slotindex);

	void InitializeMemory(FString Name);
	void SaveToFile();
	UFUNCTION(BlueprintImplementableEvent)
	void Start();

	int32 AllocateSlotIndex();

public:
	UPROPERTY(BlueprintReadWrite)
	FCharacterMemory CharacterMemory;
	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex;
};
