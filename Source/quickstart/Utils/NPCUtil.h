#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "NPCUtil.generated.h"

// Dialogue
UENUM(BlueprintType)
enum class EDialogueEventType : uint8
{
	OPENQUEST,
	GIVEITEM,
	PHASESHIFT, // 이 Dialogue를 지나는 순간, 다음에 NPC에게 말을 걸면 다음 Phase의 DialogueTree로 넘어가게 된다.
	OPENSHOP,
	MAX,
};

USTRUCT(BlueprintType)
struct FDialogueEvent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EDialogueEventType EventType;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::OPENQUEST"))
	int QuestIndex;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::GIVEITEM"))
	int ItemIndex;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex;
};

USTRUCT(BlueprintType)
struct FDialogueResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Response;
	UPROPERTY(EditAnywhere)
	bool IsEnd;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!IsEnd"))
	int NextIndex;
	UPROPERTY(EditAnywhere)
	TArray<FDialogueEvent> Events;
};

USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString Speaker;
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString TextLine;
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueResponse> Responses;
};

USTRUCT(BlueprintType)
struct FDialogueTree
{
	GENERATED_USTRUCT_BODY()

public:
	FDialogueLine GetStartLine()
	{
		return DialogueLines[StartIndex];
	}
public:
	UPROPERTY(EditAnywhere)
	TArray<FDialogueLine> DialogueLines;

	int StartIndex = 0;
};

UENUM(BlueprintType)
enum class EQuestDialogueEventType : uint8
{
	NONE,
	COMMIT,
	COMPLETE,
	GIVEITEM,
	BACKTODIALOGUE,
	PHASESHIFT,          // Quest Dialogue의 Phase shift가 아닌, NPC Normal dialogue의 시작점을 바꿔줌
	MAX,
};

USTRUCT(BlueprintType)
struct FQuestDialogueEvent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EQuestDialogueEventType EventType;
	UPROPERTY(EditAnywhere)
	int ItemIndex;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::BACKTODIALOGUE"))
	int BacktoDialogueIndex;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex;
};

USTRUCT(BlueprintType)
struct FQuestDialogueResponse
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Response;
	UPROPERTY(EditAnywhere)
	bool IsEnd;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!isEnd"))
	int NextIndex;
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueEvent> Events;
};

USTRUCT(BlueprintType)
struct FQuestDialogueLine
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Speaker;
	UPROPERTY(EditAnywhere)
	FString TextLine;
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueResponse> Responses;
};

USTRUCT(BlueprintType)
struct FQuestDialogueTree
{
	GENERATED_USTRUCT_BODY()

public:
	FQuestDialogueLine GetStartLine()
	{
		return DialogueLines[StartIndex];
	}
public:
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueLine> DialogueLines;

	int StartIndex = 0;
};

// Quest
UENUM(BlueprintType)
enum class ESingleQuestType : uint8
{
	Arrival,
	Item,
	Action,
	Hunt,
	Max,
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Serial, // Must do subquests in sequence
	Parallel, // Can do subquests parallely
	Max,
};

UENUM(BlueprintType)
enum class EQuestProgress : uint8
{
	Unavailable,
	Available,
	InProgress,
	Finished,
	AlreadyDone,
	Max,
};

UENUM(BlueprintType)
enum class ERewardType : uint8
{
	EXP,
	ITEM,
	MONEY,
	MAX,
};

USTRUCT(Atomic, BlueprintType)
struct FHuntingQuestForm
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Huntee;
	UPROPERTY(EditAnywhere)
	int HuntAmount;
};

USTRUCT(Atomic, BlueprintType)
struct FItemQuestForm
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	ETypeTag ItemType;
	UPROPERTY(EditAnywhere)
	FString ItemName;
	UPROPERTY(EditAnywhere)
	int ItemAmount;
};

USTRUCT(Atomic, BlueprintType)
struct FReward
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Reward")
	ERewardType Type;

	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::EXP"))
	float EXP;

	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::MONEY"))
	int Money;

	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::ITEM"))
	FItemShortForm Item;
};

USTRUCT(Atomic, BlueprintType)
struct FSingleQuest
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	ESingleQuestType Type;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Type == ESingleQuestType::Arrival"))
	UParticleSystem* FXTemplate;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Type == ESingleQuestType::Arrival"))
	FVector Destination;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Type == ESingleQuestType::Hunt"))
	TArray<FHuntingQuestForm> HuntingLists;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Type == ESingleQuestType::Item"))
	TArray<FItemQuestForm> ItemLists;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "Type == ESingleQuestType::Action"))
	int ActionCode;

	FQuest* Owner;

	TArray<int> currAmounts;

	bool Completed = false;

	bool CheckCompletion();
};

USTRUCT(Atomic, BlueprintType)
struct FQuest
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere, meta=(MultiLine="true"))
	FString Info;

	UPROPERTY(EditAnywhere)
	EQuestType Type;

	UPROPERTY(EditAnywhere)
	TArray<FSingleQuest> SubQuests;

	UPROPERTY(EditAnywhere)
	AActor* Instigator;

	UPROPERTY(EditAnywhere)
	EQuestProgress Progress;

	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Unavailable;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Available;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_InProgress;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Finished;

	UPROPERTY(EditAnywhere)
	TArray<FReward> Rewards;

	int currPhase = 0;

	int Remains = -1;

	FQuestDialogueLine GetStartLine();

	bool EndSingleTask();
	void UndoSingleTask();
};

// Shop
USTRUCT(Atomic, BlueprintType)
struct FShopItemShortForm
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FItemShortForm ItemInfo;
	UPROPERTY(EditAnywhere)
	int Price;
};

USTRUCT(Atomic, BlueprintType)
struct FShopItemForm
{
	GENERATED_USTRUCT_BODY()

	FItemForm ItemForm;
	int Price;
};