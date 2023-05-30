#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "NPCUtil.generated.h"

UENUM(BlueprintType)
enum class EDialogueEndType : uint8
{
	SHOP,
	OPENQUEST,
	REWARD,
	GIVEQUEST,
	DEFAULT,
	MAX,
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
	TArray<FString> Responses;
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<int> NextLines;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	bool isEnd;
	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (EditCondition = "isEnd"))
	EDialogueEndType EndContext;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EndContext == EDialogueEndType::OPENQUEST"))
	int QuestIndex;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EndContext == EDialogueEndType::REWARD"))
	int ItemIndex;
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

	UPROPERTY(EditAnywhere, Category="Quest")
	ESingleQuestType Type;

	UPROPERTY(EditAnywhere, Category = "Quest")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Quest", meta = (EditCondition = "Type == ESingleQuestType::Arrival"))
	FVector Destination;

	UPROPERTY(EditAnywhere, Category = "Quest", meta = (EditCondition = "Type == ESingleQuestType::Hunt"))
	TArray<TSubclassOf<AActor>> Huntees;
	UPROPERTY(EditAnywhere, Category = "Quest", meta = (EditCondition = "Type == ESingleQuestType::Hunt"))
	TArray<int> HuntAmounts;

	UPROPERTY(EditAnywhere, Category = "Quest", meta = (EditCondition = "Type == ESingleQuestType::Item"))
	TArray<FString> ItemNames;
	UPROPERTY(EditAnywhere, Category = "Quest", meta = (EditCondition = "Type == ESingleQuestType::Item"))
	TArray<int> ItemAmounts;

	UPROPERTY(EditAnywhere, Category = "Quest", meta = (EditCondition = "Type == ESingleQuestType::Action"))
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