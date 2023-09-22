// NPC ���� ����ü ����

#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "NPCUtil.generated.h"

// Dialogue Event�� Ÿ��
UENUM(BlueprintType)
enum class EDialogueEventType : uint8
{
	OPENQUEST,	// ����Ʈ ���̾�α׸� ��
	GIVEITEM,	// NPC�� ������ �������� �÷��̾�� ����
	PHASESHIFT, // �� Dialogue�� ������ ����, ������ NPC���� ���� �ɸ� ���� Phase�� DialogueTree�� �Ѿ�� ��
	OPENSHOP,	// ���� UI�� ��
	MAX,
};

// �� ���̾�α��� ����(����) �� �߻��ϴ� �̺�Ʈ
USTRUCT(BlueprintType)
struct FDialogueEvent
{
	GENERATED_USTRUCT_BODY()

	// �̺�Ʈ Ÿ��
	UPROPERTY(EditAnywhere)
	EDialogueEventType EventType;
	// �� ����Ʈ�� �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::OPENQUEST"))
	int QuestIndex;	
	// �� �������� �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::GIVEITEM"))
	int ItemIndex;
	// Phase Shift�� �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex;
};

// ���̾�α��� ����
USTRUCT(BlueprintType)
struct FDialogueResponse
{
	GENERATED_USTRUCT_BODY()

	// ���� �ؽ�Ʈ
	UPROPERTY(EditAnywhere)
	FString Response;
	// ���̾�α� â�� ���� ���ΰ�?
	UPROPERTY(EditAnywhere)
	bool IsEnd;
	// �ҷ��� ���̾�α� �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!IsEnd"))
	int NextIndex;
	// ���信 ���� �߻��ϴ� �̺�Ʈ
	UPROPERTY(EditAnywhere)
	TArray<FDialogueEvent> Events;
};

// ���̾�α�
USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_USTRUCT_BODY()

public:
	// ȭ��
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString Speaker;
	// ���̾�α� �ؽ�Ʈ
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FString TextLine;
	// ���� ������
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FDialogueResponse> Responses;
};

// ���̾�α� Ʈ��(�迭)
USTRUCT(BlueprintType)
struct FDialogueTree
{
	GENERATED_USTRUCT_BODY()

public:
	// ���̾�α� Ʈ���� ���� ������ ��ȯ
	FDialogueLine GetStartLine()
	{
		return DialogueLines[StartIndex];
	}
public:
	// ���̾�α� �迭
	UPROPERTY(EditAnywhere)
	TArray<FDialogueLine> DialogueLines;
	// ���� �ε���
	int StartIndex = 0;
};

// ����Ʈ ���̾�α��� �̺�Ʈ ����
UENUM(BlueprintType)
enum class EQuestDialogueEventType : uint8
{
	NONE,
	COMMIT,				// ����Ʈ�� �÷��̾�� ���
	COMPLETE,			// ����Ʈ�� �Ϸ�	
	GIVEITEM,			// NPC�� �������� �÷��̾�� ����
	BACKTODIALOGUE,		// NPC ���̾�α׷� UI�� ��ȯ
	PHASESHIFT,         // ���̾�α��� ���� �ε����� ��ȯ (���� ��ȭ���� ����, ����Ʈ ���̾�αװ� �ƴ�, ��ȭ ���� NPC�� ���̾�α׿� ����)
	MAX,
};

// ����Ʈ ���̾�α��� �̺�Ʈ
USTRUCT(BlueprintType)
struct FQuestDialogueEvent
{
	GENERATED_USTRUCT_BODY()
	// �̺�Ʈ ����
	UPROPERTY(EditAnywhere)
	EQuestDialogueEventType EventType;
	UPROPERTY(EditAnywhere)
	// �� �������� �ε���
	int ItemIndex;
	// �� NPC ���̾�α��� �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::BACKTODIALOGUE"))
	int BacktoDialogueIndex;
	// Phase Shift�� �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EventType == EQuestDialogueEventType::PHASESHIFT"))
	int NextPhaseIndex;
};

// ����Ʈ ���̾�α��� ����
USTRUCT(BlueprintType)
struct FQuestDialogueResponse
{
	GENERATED_USTRUCT_BODY()

public:
	// ���� �ؽ�Ʈ
	UPROPERTY(EditAnywhere)
	FString Response;
	// ��ȭ UI�� ������ ���ΰ�?
	UPROPERTY(EditAnywhere)
	bool IsEnd;
	// ���� ���̾�α��� �ε���
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!isEnd"))
	int NextIndex;
	// ������ �̺�Ʈ
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueEvent> Events;
};

// ����Ʈ ���̾�α�
USTRUCT(BlueprintType)
struct FQuestDialogueLine
{
	GENERATED_USTRUCT_BODY()

public:
	// ȭ��
	UPROPERTY(EditAnywhere)
	FString Speaker;
	// ���̾�α� �ؽ�Ʈ
	UPROPERTY(EditAnywhere)
	FString TextLine;
	// ���� ������
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueResponse> Responses;
};

// ����Ʈ ���̾�α� Ʈ��(�迭)
USTRUCT(BlueprintType)
struct FQuestDialogueTree
{
	GENERATED_USTRUCT_BODY()

public:
	// ���� ���̾�α׸� ��ȯ
	FQuestDialogueLine GetStartLine()
	{
		return DialogueLines[StartIndex];
	}
public:
	// ���̾�α� �迭
	UPROPERTY(EditAnywhere)
	TArray<FQuestDialogueLine> DialogueLines;
	// ���� �ε���
	int StartIndex = 0;
};

// ���� ����Ʈ Ÿ��
UENUM(BlueprintType)
enum class ESingleQuestType : uint8
{
	Arrival,	// ���� ����Ʈ
	Item,		// ������ ����Ʈ
	Action,		// �׼� ����Ʈ
	Hunt,		// óġ ����Ʈ
	Max,
};

// ����Ʈ Ÿ��
UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Serial,		// ������� �����ϴ� ����Ʈ
	Parallel,	// ���ÿ� ����Ǵ� ����Ʈ
	Max,
};

// ����Ʈ�� ���൵
UENUM(BlueprintType)
enum class EQuestProgress : uint8
{
	Unavailable,	// ���� ������ �� ����
	Available,		// ���� ������
	InProgress,		// ������
	Finished,		// �Ϸ� ������
	AlreadyDone,	// �Ϸ���
	Max,
};

// ����Ʈ�� ���� Ÿ��
UENUM(BlueprintType)
enum class ERewardType : uint8
{
	EXP,	// ����ġ
	ITEM,	// ������
	MONEY,	// ��
	MAX,
};

// óġ ����Ʈ�� ������ ���� ����ü
USTRUCT(Atomic, BlueprintType)
struct FHuntingQuestForm
{
	GENERATED_USTRUCT_BODY()
	// óġ�ؾ� �� ���� �� �̸�
	UPROPERTY(EditAnywhere)
	FString Huntee;
	// óġ�ؾ� �� ��
	UPROPERTY(EditAnywhere)
	int HuntAmount;
};

// ������ ����Ʈ�� ������ ���� ����ü
USTRUCT(Atomic, BlueprintType)
struct FItemQuestForm
{
	GENERATED_USTRUCT_BODY()
	// ȹ���� �������� Ÿ��
	UPROPERTY(EditAnywhere)
	ETypeTag ItemType;
	// ȹ���� �������� �̸�
	UPROPERTY(EditAnywhere)
	FString ItemName;
	// ȹ���ؾ� �� ��
	UPROPERTY(EditAnywhere)
	int ItemAmount;
};

// ����Ʈ�� ����
USTRUCT(Atomic, BlueprintType)
struct FReward
{
	GENERATED_USTRUCT_BODY()
	// ���� Ÿ��
	UPROPERTY(EditAnywhere, Category = "Reward")
	ERewardType Type;
	// ����ġ
	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::EXP"))
	float EXP;
	// ��
	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::MONEY"))
	int Money;
	// ������
	UPROPERTY(EditAnywhere, Category = "Reward", meta = (EditCondition = "Type == ERewardType::ITEM"))
	FItemShortForm Item;
};

// ���� ����Ʈ
USTRUCT(Atomic, BlueprintType)
struct FSingleQuest
{
	GENERATED_USTRUCT_BODY()
	// Ÿ��
	UPROPERTY(BlueprintReadWrite)
	ESingleQuestType Type;
	// �̸�
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	// ���� ����Ʈ - ���� �̸�
	UPROPERTY(BlueprintReadWrite)
	FString MapName;
	// ���� ����Ʈ - ���� ��ġ
	UPROPERTY(BlueprintReadWrite)
	FVector Destination;
	// ���� ����Ʈ - ��ġ�� ǥ���� FX
	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* FXTemplate = nullptr;
	// óġ ����Ʈ - óġ ���
	UPROPERTY(BlueprintReadWrite)
	TArray<FHuntingQuestForm> HuntingLists;
	// ������ ����Ʈ - ȹ�� ���
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemQuestForm> ItemLists;
	// �׼� ����Ʈ - �ڵ� ��ȣ
	UPROPERTY(BlueprintReadWrite)
	int ActionCode;
	// �׼� ����Ʈ - ����Ʈ ����
	UPROPERTY(BlueprintReadWrite)
	FString ActionInfo;

	// ���� ����Ʈ�� �Ҽӵ� ���� ����Ʈ
	FQuest* Owner;

	// óġ/ȹ�� ����Ʈ - ������� óġ/ȹ���� ��
	TArray<int> currAmounts;
	// �Ϸ� ����
	bool Completed = false;

	// �Ϸ� ���θ� ����
	bool CheckCompletion();
};

// ����Ʈ
USTRUCT(Atomic, BlueprintType)
struct FQuest
{
	GENERATED_USTRUCT_BODY()
	// ����Ʈ ��ȣ
	UPROPERTY(EditAnywhere)
	int32 Index;
	// ����Ʈ �̸�
	UPROPERTY(EditAnywhere)
	FString Name;
	// ����Ʈ ����
	UPROPERTY(EditAnywhere, meta=(MultiLine="true"))
	FString Info;
	// ����Ʈ Ÿ��
	UPROPERTY(EditAnywhere)
	EQuestType Type;
	// ���� ����Ʈ ���
	UPROPERTY(EditAnywhere)
	TArray<FSingleQuest> SubQuests;
	// ����Ʈ�� �ñ� NPC
	UPROPERTY(EditAnywhere)
	FString Instigator;
	// ����Ʈ�� �Ϸ��ų NPC
	UPROPERTY(EditAnywhere)
	FString Ender;
	// ����Ʈ ���൵
	UPROPERTY(EditAnywhere)
	EQuestProgress Progress;

	// ���൵�� ���� ��ȭ ���
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Unavailable;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Available;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_InProgress;
	UPROPERTY(EditAnywhere)
	FQuestDialogueTree DialogueTree_Finished;

	// ���� ���
	UPROPERTY(EditAnywhere)
	TArray<FReward> Rewards;

	// Serial Quest - ���� �������� ���� ����Ʈ �ε���
	int CurrPhase = 0;
	// Parallel Quest - ������� �Ϸ��� ���� ����Ʈ ��
	int Remains = -1;

	// NPC�� �� ����Ʈ�� Instigator�� Ender�� ���, ���൵�� �´� ���̾�α��� �������� ��ȯ
	FQuestDialogueLine GetStartLine(FString NPCName);

	// ���� ����Ʈ�� �Ϸ�
	bool EndSingleTask();
	// �Ϸ�� ���� ����Ʈ�� �ٽ� �������� (ex : ȹ���� �������� ���� �پ��� ��)
	void UndoSingleTask();
};

// ���� ����Ʈ�� ���� ��Ȳ�� �����ϴ� ����ü
USTRUCT(BlueprintType)
struct FSubQuestStatus
{
	GENERATED_USTRUCT_BODY()
	// �Ϸ� ����
	bool Completed;
	// óġ/������ ����Ʈ - ������� óġ/ȹ���� ��
	TArray<int> CurrAmount;
};

// ����Ʈ�� ���� ��Ȳ�� �����ϴ� ����ü
USTRUCT(BlueprintType)
struct FQuestStatus
{
	GENERATED_USTRUCT_BODY()
	// ����Ʈ�� �ε���
	int QuestIndex;
	// ���൵
	EQuestProgress Progress;
	// Serail Quest - ���� �������� ���� ����Ʈ �ε���
	int CurrPhase;
	// Parallel Quest - ������� �Ϸ��� ���� ����Ʈ ��
	int Remains;
	// ���� ����Ʈ�� ���൵
	TArray<FSubQuestStatus> SubStatus;
};

// ���� �������� ������ ���� ����ü
USTRUCT(Atomic, BlueprintType)
struct FShopItemShortForm
{
	GENERATED_USTRUCT_BODY()
	// �������� ����(�̸�, Ÿ��, ����)
	UPROPERTY(EditAnywhere)
	FItemShortForm ItemInfo;
	// �������� ���� ����
	UPROPERTY(EditAnywhere)
	int Price;
};

// ���� ������ ���� + ����� �̹���
USTRUCT(Atomic, BlueprintType)
struct FShopItemForm
{
	GENERATED_USTRUCT_BODY()
	// ������ ���� + ����� �̹���
	FItemForm ItemForm;
	// �� ����
	int Price;
};

// �÷��̾ �������� ����Ʈ�� �����ϴ� ����ü
USTRUCT(BlueprintType)
struct FWorkingQuestMemory
{
	GENERATED_USTRUCT_BODY()

	// �������� ����Ʈ ���
	TArray<FQuest*> WorkingQuests;
	// �������� ���� ����Ʈ�� Ÿ�Ժ��� ����
	TArray<FSingleQuest*> HuntingQuests;
	TArray<FSingleQuest*> ArrivalQuests;
	TArray<FSingleQuest*> ItemQuests;
	TArray<FSingleQuest*> ActionQuests;
};