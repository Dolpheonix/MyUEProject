// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../../Utils/Structs.h"
#include "GameSaver.generated.h"

#define MAX_SAVE_SLOTS 10

USTRUCT(BlueprintType)
struct FCharacterMemory
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int32 CurrentLevel;
	UPROPERTY(EditDefaultsOnly)
	FString CurrentMap;
	UPROPERTY(EditDefaultsOnly)
	FVector CurrentPos;
	UPROPERTY(EditDefaultsOnly)
	FRotator CurrentRot;
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	UPROPERTY(EditDefaultsOnly)
	float HP;
	UPROPERTY(EditDefaultsOnly)
	int32 Money;
	UPROPERTY(EditDefaultsOnly)
	TArray<FWrappedItemForm> Inventory;
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Before;
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Now;
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Next;
};

USTRUCT(BlueprintType)
struct FMapMemory
{
	GENERATED_USTRUCT_BODY()

public:
	int32 a;
};

UCLASS()
class QUICKSTART_API USaveSlots : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FString> SlotNames;
};

UCLASS()
class QUICKSTART_API UGameSaver : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	int32 SlotIndex;

	UPROPERTY(EditDefaultsOnly)
	FCharacterMemory CharacterMemory;
};
