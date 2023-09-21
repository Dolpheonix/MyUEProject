///////////////////GameSaver/////////////
// ���̺�/�ε� �� ����ϴ� ������ ����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../../Utils/Structs.h"
#include "../../Utils/NPCUtil.h"
#include "GameSaver.generated.h"

#define MAX_SAVE_SLOTS 10

// ĳ���� �����͸� ����
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
	// �������� ���� ������
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Before;
	// �������� ���� ������
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Now;
	// �������� ���� ������
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Quickslots_Next;
};

// �Ϲ����� ������ ������ ����
USTRUCT(BlueprintType)
struct FActorMemory
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	// ���� ����
	UPROPERTY(EditDefaultsOnly)
	bool IsValid;
	UPROPERTY(EditDefaultsOnly)
	FVector CurrentPos;
	UPROPERTY(EditDefaultsOnly)
	FRotator CurrentRot;
};

// NPC�� ������
USTRUCT(BlueprintType)
struct FNPCMemory
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FString DisplayName;
	// �⺻ ���� ������
	UPROPERTY(EditDefaultsOnly)
	FActorMemory ActorMemory;
	// ��ȭ ��������
	UPROPERTY(EditDefaultsOnly)
	int DialoguePhase;
	// ���� ����
	UPROPERTY(EditDefaultsOnly)
	TArray<FShopItemShortForm> ShopItems;
	// ���� ���� ���� ������ ����
	UPROPERTY(EditDefaultsOnly)
	TArray<FItemShortForm> AcquireItems;
};

// �� ������
USTRUCT(BlueprintType)
struct FMapMemory
{
	GENERATED_USTRUCT_BODY()

public:
	// �� �̸�
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	// NPC �޸�
	UPROPERTY(EditDefaultsOnly)
	TArray<FNPCMemory> LocalNPCMemory;
	// ���� ������ �޸�
	UPROPERTY(EditDefaultsOnly)
	TArray<FActorMemory> OriginActorMemory_CollectableItem;
	// Dynamic ������Ʈ �޸�
	UPROPERTY(EditDefaultsOnly)
	TArray<FActorMemory> DynamicActorMemory;
};
