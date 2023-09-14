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
	PHASESHIFT, // �� Dialogue�� ������ ����, ������ NPC���� ���� �ɸ� ���� Phase�� DialogueTree�� �Ѿ�� �ȴ�.
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
	int QuestIndex;		// �ҷ��� ����Ʈ ��ȣ
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::GIVEITEM"))
	int ItemIndex;		// ������ �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex; // ����Ʈ�� ��ȭ �ε���
};

USTRUCT(BlueprintType)
struct FDialogueResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Response;	// ���� ����
	UPROPERTY(EditAnywhere)
	bool IsEnd;			// ���̾�α� â�� �������ΰ�?
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!IsEnd"))
	int NextIndex;		// ���� ��ȭ �ε���
	UPROPERTY(EditAnywhere)
	TArray<FDialogueEvent> Events;	// ��ȭ �̺�Ʈ
};

USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString Speaker;	// ȭ��(NPC)
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString TextLine;	// NPC�� ��ȭ ����
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueResponse> Responses; // �÷��̾��� ���� ���
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
	PHASESHIFT,          // Quest Dialogue�� Phase shift�� �ƴ�, NPC Normal dialogue�� �������� �ٲ���
	MAX,
};

USTRUCT(BlueprintType)
struct FQuestDialogueEvent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EQuestDialogueEventType EventType;
	UPROPERTY(EditAnywhere)
	int ItemIndex;	// ������ �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::BACKTODIALOGUE"))
	int BacktoDialogueIndex; // ���ư� ��ȭ �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex; // ����Ʈ�� ��ȭ �ε���
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
	FString Speaker;	// ȭ��
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
	FString Huntee;
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

	UPROPERTY(BlueprintReadWrite)
	ESingleQuestType Type;		// Ÿ��

	UPROPERTY(BlueprintReadWrite)
	FString Name;				// �̸�
	// Arrival
	UPROPERTY(BlueprintReadWrite)
	FString MapName;			// �� �̸�
	UPROPERTY(BlueprintReadWrite)
	FVector Destination;		// ���� ��ġ
	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* FXTemplate = nullptr; // ��ġ�� ǥ���� ��ƼŬ
	// Hunt
	UPROPERTY(BlueprintReadWrite)
	TArray<FHuntingQuestForm> HuntingLists;
	// Item
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemQuestForm> ItemLists;
	// Action
	UPROPERTY(BlueprintReadWrite)
	int ActionCode;				// �ڵ�
	UPROPERTY(BlueprintReadWrite)
	FString ActionInfo;			// ����

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
	int32 Index;			// �ĺ���ȣ
	UPROPERTY(EditAnywhere)
	FString Name;			// �̸�
	UPROPERTY(EditAnywhere, meta=(MultiLine="true"))
	FString Info;			// ����
	UPROPERTY(EditAnywhere)
	EQuestType Type;		// Ÿ��
	UPROPERTY(EditAnywhere)
	TArray<FSingleQuest> SubQuests;	// ��������Ʈ
	UPROPERTY(EditAnywhere)
	FString Instigator;		// �ñ� NPC
	UPROPERTY(EditAnywhere)
	FString Ender;			// �Ϸ� NPC
	UPROPERTY(EditAnywhere)
	EQuestProgress Progress;// ���� ��Ȳ
	// ���൵�� ���� ��ȭ ���
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Unavailable;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Available;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_InProgress;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Finished;
	// ����
	UPROPERTY(EditAnywhere)
	TArray<FReward> Rewards;

	int CurrPhase = 0;

	int Remains = -1;

	FQuestDialogueLine GetStartLine(FString NPCName);

	bool EndSingleTask();
	void UndoSingleTask();
};

USTRUCT(BlueprintType)
struct FSubQuestStatus
{
	GENERATED_USTRUCT_BODY()

	bool Completed;
	TArray<int> CurrAmount;
};

USTRUCT(BlueprintType)
struct FQuestStatus
{
	GENERATED_USTRUCT_BODY()

	int QuestIndex;
	EQuestProgress Progress;
	int CurrPhase;
	int Remains;
	TArray<FSubQuestStatus> SubStatus;
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

USTRUCT(BlueprintType)
struct FWorkingQuestMemory
{
	GENERATED_USTRUCT_BODY()

	TArray<FQuest*> WorkingQuests;
	TArray<FSingleQuest*> HuntingQuests;
	TArray<FSingleQuest*> ArrivalQuests;
	TArray<FSingleQuest*> ItemQuests;
	TArray<FSingleQuest*> ActionQuests;
};