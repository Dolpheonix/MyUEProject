#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "NPCUtil.generated.h"

UENUM(BlueprintType)
enum class EDialogueEndType : uint8
{
	DEFAULT,
	SHOP,
	QUEST_START,
	QUEST_COMMIT,
	QUEST_END,
	MAX,
};

USTRUCT(BlueprintType)
struct FDialogueResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Response;
	UPROPERTY(EditAnywhere)
	bool isEnd;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!isEnd"))
	int nextIndex;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "isEnd"))
	EDialogueEndType EndContext;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EndContext == EDialogueEndType::QUEST_START"))
	int QuestIndex;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EndContext == EDialogueEndType::REWARD"))
	int ItemIndex;
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
	Serial,
	Parallel,
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

	UPROPERTY(EditAnywhere, Category = "Reward")
	float EXP;

	UPROPERTY(EditAnywhere, Category = "Reward")
	int Money;

	UPROPERTY(EditAnywhere, Category = "Reward")
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

	UPROPERTY(EditAnywhere, Category = "Quest")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Quest")
	EQuestType Type;

	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<FSingleQuest> SubQuests;

	UPROPERTY(EditAnywhere, Category = "Quest")
	AActor* Instigator;

	UPROPERTY(EditAnywhere, Category = "Quest")
	EQuestProgress Progress;

	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<FDialogueLine> CommitDialogue;

	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<FDialogueLine> InProgressDialogue;

	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<FDialogueLine> FinishDialogue;

	UPROPERTY(EditAnywhere, Category = "Quest")
	TArray<FReward> Rewards;

	int currPhase = 0;

	int Remains = -1;
};

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