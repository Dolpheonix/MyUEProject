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
	int QuestIndex;		// 불러올 퀘스트 번호
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::GIVEITEM"))
	int ItemIndex;		// 아이템 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex; // 시프트할 대화 인덱스
};

USTRUCT(BlueprintType)
struct FDialogueResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Response;	// 응답 내용
	UPROPERTY(EditAnywhere)
	bool IsEnd;			// 다이얼로그 창을 닫을것인가?
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!IsEnd"))
	int NextIndex;		// 다음 대화 인덱스
	UPROPERTY(EditAnywhere)
	TArray<FDialogueEvent> Events;	// 대화 이벤트
};

USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString Speaker;	// 화자(NPC)
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString TextLine;	// NPC의 대화 내용
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueResponse> Responses; // 플레이어의 응답 목록
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
	int ItemIndex;	// 아이템 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::BACKTODIALOGUE"))
	int BacktoDialogueIndex; // 돌아갈 대화 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex; // 시프트할 대화 인덱스
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
	FString Speaker;	// 화자
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
	ESingleQuestType Type;		// 타입

	UPROPERTY(BlueprintReadWrite)
	FString Name;				// 이름
	// Arrival
	UPROPERTY(BlueprintReadWrite)
	FString MapName;			// 맵 이름
	UPROPERTY(BlueprintReadWrite)
	FVector Destination;		// 도착 위치
	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* FXTemplate = nullptr; // 위치를 표시할 파티클
	// Hunt
	UPROPERTY(BlueprintReadWrite)
	TArray<FHuntingQuestForm> HuntingLists;
	// Item
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemQuestForm> ItemLists;
	// Action
	UPROPERTY(BlueprintReadWrite)
	int ActionCode;				// 코드
	UPROPERTY(BlueprintReadWrite)
	FString ActionInfo;			// 설명

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
	int32 Index;			// 식별번호
	UPROPERTY(EditAnywhere)
	FString Name;			// 이름
	UPROPERTY(EditAnywhere, meta=(MultiLine="true"))
	FString Info;			// 설명
	UPROPERTY(EditAnywhere)
	EQuestType Type;		// 타입
	UPROPERTY(EditAnywhere)
	TArray<FSingleQuest> SubQuests;	// 서브퀘스트
	UPROPERTY(EditAnywhere)
	FString Instigator;		// 맡긴 NPC
	UPROPERTY(EditAnywhere)
	FString Ender;			// 완료 NPC
	UPROPERTY(EditAnywhere)
	EQuestProgress Progress;// 진행 상황
	// 진행도에 따른 대화 목록
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Unavailable;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Available;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_InProgress;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Finished;
	// 보상
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