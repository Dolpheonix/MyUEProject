// NPC 관련 구조체 모음

#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "NPCUtil.generated.h"

// Dialogue Event의 타입
UENUM(BlueprintType)
enum class EDialogueEventType : uint8
{
	OPENQUEST,	// 퀘스트 다이얼로그를 엶
	GIVEITEM,	// NPC가 보유한 아이템을 플레이어에게 전달
	PHASESHIFT, // 이 Dialogue를 지나는 순간, 다음에 NPC에게 말을 걸면 다음 Phase의 DialogueTree로 넘어가게 됨
	OPENSHOP,	// 상점 UI를 엶
	MAX,
};

// 한 다이얼로그의 종료(응답) 시 발생하는 이벤트
USTRUCT(BlueprintType)
struct FDialogueEvent
{
	GENERATED_USTRUCT_BODY()

	// 이벤트 타입
	UPROPERTY(EditAnywhere)
	EDialogueEventType EventType;
	// 열 퀘스트의 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::OPENQUEST"))
	int QuestIndex;	
	// 줄 아이템의 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::GIVEITEM"))
	int ItemIndex;
	// Phase Shift할 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex;
};

// 다이얼로그의 응답
USTRUCT(BlueprintType)
struct FDialogueResponse
{
	GENERATED_USTRUCT_BODY()

	// 응답 텍스트
	UPROPERTY(EditAnywhere)
	FString Response;
	// 다이얼로그 창을 닫을 것인가?
	UPROPERTY(EditAnywhere)
	bool IsEnd;
	// 불러올 다이얼로그 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!IsEnd"))
	int NextIndex;
	// 응답에 의해 발생하는 이벤트
	UPROPERTY(EditAnywhere)
	TArray<FDialogueEvent> Events;
};

// 다이얼로그
USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_USTRUCT_BODY()

public:
	// 화자
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString Speaker;
	// 다이얼로그 텍스트
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString TextLine;
	// 응답 선택지
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueResponse> Responses;
};

// 다이얼로그 트리(배열)
USTRUCT(BlueprintType)
struct FDialogueTree
{
	GENERATED_USTRUCT_BODY()

public:
	// 다이얼로그 트리의 시작 지점을 반환
	FDialogueLine GetStartLine()
	{
		return DialogueLines[StartIndex];
	}
public:
	// 다이얼로그 배열
	UPROPERTY(EditAnywhere)
	TArray<FDialogueLine> DialogueLines;
	// 시작 인덱스
	int StartIndex = 0;
};

// 퀘스트 다이얼로그의 이벤트 종류
UENUM(BlueprintType)
enum class EQuestDialogueEventType : uint8
{
	NONE,
	COMMIT,				// 퀘스트를 플레이어에게 등록
	COMPLETE,			// 퀘스트를 완료	
	GIVEITEM,			// NPC의 아이템을 플레이어에게 전달
	BACKTODIALOGUE,		// NPC 다이얼로그로 UI를 전환
	PHASESHIFT,         // 다이얼로그의 시작 인덱스를 변환 (다음 대화부터 적용, 퀘스트 다이얼로그가 아닌, 대화 중인 NPC의 다이얼로그에 적용)
	MAX,
};

// 퀘스트 다이얼로그의 이벤트
USTRUCT(BlueprintType)
struct FQuestDialogueEvent
{
	GENERATED_USTRUCT_BODY()
	// 이벤트 종류
	UPROPERTY(EditAnywhere)
	EQuestDialogueEventType EventType;
	UPROPERTY(EditAnywhere)
	// 줄 아이템의 인덱스
	int ItemIndex;
	// 열 NPC 다이얼로그의 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::BACKTODIALOGUE"))
	int BacktoDialogueIndex;
	// Phase Shift할 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex;
};

// 퀘스트 다이얼로그의 응답
USTRUCT(BlueprintType)
struct FQuestDialogueResponse
{
	GENERATED_USTRUCT_BODY()

public:
	// 응답 텍스트
	UPROPERTY(EditAnywhere)
	FString Response;
	// 대화 UI를 종료할 것인가?
	UPROPERTY(EditAnywhere)
	bool IsEnd;
	// 다음 다이얼로그의 인덱스
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!isEnd"))
	int NextIndex;
	// 응답의 이벤트
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueEvent> Events;
};

// 퀘스트 다이얼로그
USTRUCT(BlueprintType)
struct FQuestDialogueLine
{
	GENERATED_USTRUCT_BODY()

public:
	// 화자
	UPROPERTY(EditAnywhere)
	FString Speaker;
	// 다이얼로그 텍스트
	UPROPERTY(EditAnywhere)
	FString TextLine;
	// 응답 선택지
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueResponse> Responses;
};

// 퀘스트 다이얼로그 트리(배열)
USTRUCT(BlueprintType)
struct FQuestDialogueTree
{
	GENERATED_USTRUCT_BODY()

public:
	// 시작 다이얼로그를 반환
	FQuestDialogueLine GetStartLine()
	{
		return DialogueLines[StartIndex];
	}
public:
	// 다이얼로그 배열
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueLine> DialogueLines;
	// 시작 인덱스
	int StartIndex = 0;
};

// 서브 퀘스트 타입
UENUM(BlueprintType)
enum class ESingleQuestType : uint8
{
	Arrival,	// 도착 퀘스트
	Item,		// 아이템 퀘스트
	Action,		// 액션 퀘스트
	Hunt,		// 처치 퀘스트
	Max,
};

// 퀘스트 타입
UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Serial,		// 순서대로 진행하는 퀘스트
	Parallel,	// 동시에 진행되는 퀘스트
	Max,
};

// 퀘스트의 진행도
UENUM(BlueprintType)
enum class EQuestProgress : uint8
{
	Unavailable,	// 아직 수행할 수 없음
	Available,		// 수행 가능함
	InProgress,		// 진행중
	Finished,		// 완료 가능함
	AlreadyDone,	// 완료함
	Max,
};

// 퀘스트의 보상 타입
UENUM(BlueprintType)
enum class ERewardType : uint8
{
	EXP,	// 경험치
	ITEM,	// 아이템
	MONEY,	// 돈
	MAX,
};

// 처치 퀘스트의 내용을 담은 구조체
USTRUCT(Atomic, BlueprintType)
struct FHuntingQuestForm
{
	GENERATED_USTRUCT_BODY()
	// 처치해야 할 적의 라벨 이름
	UPROPERTY(EditAnywhere)
	FString Huntee;
	// 처치해야 할 수
	UPROPERTY(EditAnywhere)
	int HuntAmount;
};

// 아이템 퀘스트의 내용을 담은 구조체
USTRUCT(Atomic, BlueprintType)
struct FItemQuestForm
{
	GENERATED_USTRUCT_BODY()
	// 획득할 아이템의 타입
	UPROPERTY(EditAnywhere)
	ETypeTag ItemType;
	// 획득할 아이템의 이름
	UPROPERTY(EditAnywhere)
	FString ItemName;
	// 획득해야 할 수
	UPROPERTY(EditAnywhere)
	int ItemAmount;
};

// 퀘스트의 보상
USTRUCT(Atomic, BlueprintType)
struct FReward
{
	GENERATED_USTRUCT_BODY()
	// 보상 타입
	UPROPERTY(EditAnywhere, Category = "Reward")
	ERewardType Type;
	// 경험치
	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::EXP"))
	float EXP;
	// 돈
	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::MONEY"))
	int Money;
	// 아이템
	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::ITEM"))
	FItemShortForm Item;
};

// 서브 퀘스트
USTRUCT(Atomic, BlueprintType)
struct FSingleQuest
{
	GENERATED_USTRUCT_BODY()
	// 타입
	UPROPERTY(BlueprintReadWrite)
	ESingleQuestType Type;
	// 이름
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	// 도착 퀘스트 - 맵의 이름
	UPROPERTY(BlueprintReadWrite)
	FString MapName;
	// 도착 퀘스트 - 도착 위치
	UPROPERTY(BlueprintReadWrite)
	FVector Destination;
	// 도착 퀘스트 - 위치를 표시할 FX
	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* FXTemplate = nullptr;
	// 처치 퀘스트 - 처치 목록
	UPROPERTY(BlueprintReadWrite)
	TArray<FHuntingQuestForm> HuntingLists;
	// 아이템 퀘스트 - 획득 목록
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemQuestForm> ItemLists;
	// 액션 퀘스트 - 코드 번호
	UPROPERTY(BlueprintReadWrite)
	int ActionCode;
	// 액션 퀘스트 - 퀘스트 설명
	UPROPERTY(BlueprintReadWrite)
	FString ActionInfo;

	// 서브 퀘스트가 소속된 메인 퀘스트
	FQuest* Owner;

	// 처치/획득 퀘스트 - 현재까지 처치/획득한 수
	TArray<int> currAmounts;
	// 완료 여부
	bool Completed = false;

	// 완료 여부를 갱신
	bool CheckCompletion();
};

// 퀘스트
USTRUCT(Atomic, BlueprintType)
struct FQuest
{
	GENERATED_USTRUCT_BODY()
	// 퀘스트 번호
	UPROPERTY(EditAnywhere)
	int32 Index;
	// 퀘스트 이름
	UPROPERTY(EditAnywhere)
	FString Name;
	// 퀘스트 정보
	UPROPERTY(EditAnywhere, meta=(MultiLine="true"))
	FString Info;
	// 퀘스트 타입
	UPROPERTY(EditAnywhere)
	EQuestType Type;
	// 서브 퀘스트 목록
	UPROPERTY(EditAnywhere)
	TArray<FSingleQuest> SubQuests;
	// 퀘스트를 맡길 NPC
	UPROPERTY(EditAnywhere)
	FString Instigator;
	// 퀘스트를 완료시킬 NPC
	UPROPERTY(EditAnywhere)
	FString Ender;
	// 퀘스트 진행도
	UPROPERTY(EditAnywhere)
	EQuestProgress Progress;

	// 진행도에 따른 대화 목록
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Unavailable;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Available;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_InProgress;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Finished;

	// 보상 목록
	UPROPERTY(EditAnywhere)
	TArray<FReward> Rewards;

	// Serial Quest - 현재 진행중인 서브 퀘스트 인덱스
	int CurrPhase = 0;
	// Parallel Quest - 현재까지 완료한 서브 퀘스트 수
	int Remains = -1;

	// NPC가 이 퀘스트의 Instigator나 Ender일 경우, 진행도에 맞는 다이얼로그의 시작점을 반환
	FQuestDialogueLine GetStartLine(FString NPCName);

	// 서브 퀘스트를 완료
	bool EndSingleTask();
	// 완료된 서브 퀘스트를 다시 돌려놓음 (ex : 획득한 아이템의 수가 줄었을 때)
	void UndoSingleTask();
};

// 서브 퀘스트의 진행 상황을 보관하는 구조체
USTRUCT(BlueprintType)
struct FSubQuestStatus
{
	GENERATED_USTRUCT_BODY()
	// 완료 여부
	bool Completed;
	// 처치/아이템 퀘스트 - 현재까지 처치/획득한 수
	TArray<int> CurrAmount;
};

// 퀘스트의 진행 상황을 보관하는 구조체
USTRUCT(BlueprintType)
struct FQuestStatus
{
	GENERATED_USTRUCT_BODY()
	// 퀘스트의 인덱스
	int QuestIndex;
	// 진행도
	EQuestProgress Progress;
	// Serail Quest - 현재 진행중인 서브 퀘스트 인덱스
	int CurrPhase;
	// Parallel Quest - 현재까지 완료한 서브 퀘스트 수
	int Remains;
	// 서브 퀘스트의 진행도
	TArray<FSubQuestStatus> SubStatus;
};

// 상점 아이템의 정보를 담은 구조체
USTRUCT(Atomic, BlueprintType)
struct FShopItemShortForm
{
	GENERATED_USTRUCT_BODY()
	// 아이템의 정보(이름, 타입, 개수)
	UPROPERTY(EditAnywhere)
	FItemShortForm ItemInfo;
	// 아이템의 개당 가격
	UPROPERTY(EditAnywhere)
	int Price;
};

// 상점 아이템 정보 + 썸네일 이미지
USTRUCT(Atomic, BlueprintType)
struct FShopItemForm
{
	GENERATED_USTRUCT_BODY()
	// 아이템 정보 + 썸네일 이미지
	FItemForm ItemForm;
	// 팔 가격
	int Price;
};

// 플레이어가 진행중인 퀘스트를 보관하는 구조체
USTRUCT(BlueprintType)
struct FWorkingQuestMemory
{
	GENERATED_USTRUCT_BODY()

	// 진행중인 퀘스트 목록
	TArray<FQuest*> WorkingQuests;
	// 진행중인 서브 퀘스트를 타입별로 관리
	TArray<FSingleQuest*> HuntingQuests;
	TArray<FSingleQuest*> ArrivalQuests;
	TArray<FSingleQuest*> ItemQuests;
	TArray<FSingleQuest*> ActionQuests;
};