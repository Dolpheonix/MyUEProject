///////////////////GameSaver/////////////
// 세이브/로드 시 사용하는 데이터 모음
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../../Utils/Structs.h"
#include "../../Utils/NPCUtil.h"
#include "GameSaver.generated.h"

#define MAX_SAVE_SLOTS 10

// 캐릭터 데이터를 저장
USTRUCT(BlueprintType)
struct FCharacterMemory
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString DisplayName;
	UPROPERTY(EditDefaultsOnly)
	int32 CurrentLevel;
	UPROPERTY(EditDefaultsOnly)
	FString CurrentMap;
	UPROPERTY(EditDefaultsOnly)
	FVector CurrentPos;
	UPROPERTY(EditDefaultsOnly)
	FRotator CurrentRot;
	UPROPERTY(EditDefaultsOnly)
	float CurrentHP;
	UPROPERTY(EditDefaultsOnly)
	int32 CurrentMoney;
	UPROPERTY(EditDefaultsOnly)
	TArray<FWrappedItemForm> Inventory;
	// 퀵슬롯의 이전 아이템
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Before;
	// 퀵슬롯의 현재 아이템
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Now;
	// 퀵슬롯의 다음 아이템
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Next;
};

// 일반적인 액터의 데이터 저장
USTRUCT(BlueprintType)
struct FActorMemory
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	// 존재 여부
	UPROPERTY(EditDefaultsOnly)
	bool IsValid;
	UPROPERTY(EditDefaultsOnly)
	FVector CurrentPos;
	UPROPERTY(EditDefaultsOnly)
	FRotator CurrentRot;
};

// NPC의 데이터
USTRUCT(BlueprintType)
struct FNPCMemory
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString DisplayName;
	// 기본 액터 데이터
	UPROPERTY(EditDefaultsOnly)
	FActorMemory ActorMemory;
	// 대화 시작지점
	UPROPERTY(EditDefaultsOnly)
	int DialoguePhase;
	// 상점 상태
	UPROPERTY(EditDefaultsOnly)
	TArray<FShopItemShortForm> ShopItems;
	// 상점 외의 보유 아이템 상태
	UPROPERTY(EditDefaultsOnly)
	TArray<FItemShortForm> AcquireItems;
};

// 맵 데이터
USTRUCT(BlueprintType)
struct FMapMemory
{
	GENERATED_USTRUCT_BODY()

public:
	// 맵 이름
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	// NPC 메모리
	UPROPERTY(EditDefaultsOnly)
	TArray<FNPCMemory> LocalNPCMemory;
	// 루팅 아이템 메모리
	UPROPERTY(EditDefaultsOnly)
	TArray<FActorMemory> OriginActorMemory_CollectableItem;
	// Dynamic 오브젝트 메모리
	UPROPERTY(EditDefaultsOnly)
	TArray<FActorMemory> DynamicActorMemory;
};
