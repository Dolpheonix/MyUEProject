/////////////UMainGameInstance/////////////////////
// ���� ���� �Բ� ���� ������ �����ϴ� ���� �ν��Ͻ��Դϴ�.
// 1. ���̺� ���� ���� ���
// 2. ���� ��ȯ �� ĳ������ ���� �ӽ� ����
// 3. ����Ʈ �ε� �� ���� ���

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../../Utils/Structs.h"
#include "../../Utils/NPCUtil.h"
#include "../../Character/Main/MainCharacter.h"
#include "../SaveObject/GameSaver.h"
#include "MainGameInstance.generated.h"

UCLASS()
class QUICKSTART_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMainGameInstance();
	
	// ���� ���۽� ȣ��
	virtual void Init() override;
	// ���� ����� ȣ�� 
	virtual void Shutdown() override;

public:
	// ĳ���� ���� �����͸� �ʱ�ȭ
	void InitializeCharacterMemory(FString Name);
	// �ҷ���(�ʱ�ȭ��) ĳ���� �����͸� ������ ���Ϳ� ���� 
	void ApplyCharacterMemory(AMainCharacter* character);
	// ĳ������ �����͸� �����ν���Ʈ�� �ӽ�����
	void SaveCharacterMemory(AMainCharacter* character);

	// �����ν��Ͻ��� ���� �����͸� ���̺����Ͽ� ����
	bool SaveToFile();
	// ����Ʈ ���¸� FQuestStatus ����ü �������� ��ȯ
	TArray<FQuestStatus> ConvertToQuestStatus();
	// �� ���¸� ����
	void SaveLevel();
	// ���� NPC ���¸� ����
	void SaveNPC();
	// ���� ���� ������ ���¸� ����
	void SaveItems();

	// ���Ϸκ��� ���� �����͸� �ҷ���
	void LoadFromFile(FString Slotname);
	// ����Ʈ ���� ��Ȳ�� ����Ʈ ������Ʈ�� ����
	// QuestStatus : ���Ϸκ��� �ҷ��� ����Ʈ �����Ȳ
	void ApplyQuestStatus(TArray<FQuestStatus> QuestStatus);
	// �� �����Ϳ� �Բ� ���� �ҷ���
	UFUNCTION(BlueprintCallable)
	void LoadLevel(FString MapName, bool isStart);
	// NPC ���¸� �ʿ� ����
	UFUNCTION()
	void LoadNPC(UWorld* world);
	// ���� ������ ���¸� �ʿ� ����
	UFUNCTION()
	void LoadItems(UWorld* world);
	// �� ���� ���̳��� ������Ʈ�� �ʿ� ����(EX. ���� ����Ʈ�� �������� ǥ�� FX)
	UFUNCTION()
	void LoadObjects(UWorld* world);

	// ������ ������
	void Start();

	// ���̺������� ���� ���� ��, �������� �ε����� ����
	int32 AllocateSlotIndex();

	// �ε����� �´� ����Ʈ ����
	FQuest* GetQuest(int index);

public:
	// ����Ʈ�� ���� �� ���¸� ����(�����÷��� ����)
	UPROPERTY(BlueprintReadWrite)
	TArray<FQuest> Quests;
	// ĳ���� ���� ������ (���� ��ȯ�� �ӽ������)
	UPROPERTY(BlueprintReadWrite)
	FCharacterMemory CharacterMemory;
	// �÷��̾ ���� �������� ����Ʈ ��� (���� ��ȯ�� �ӽ������)
	UPROPERTY(BlueprintReadWrite)
	FWorkingQuestMemory CharacterMemory_Quest;
	// ��ü �� ������ (���� �ε�� �ʿ� ����)
	UPROPERTY(BlueprintReadWrite)
	TArray<FMapMemory> MapMemories;
	// ���� ���� �����͸� ����
	FMapMemory* CurrentMapMemory;
	// ���� �÷��̾��� ���̺� ���� ��ȣ
	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex;

	// Init() �Լ� ���� ȣ�� ���� Ȯ��
	bool bApplyBound = false;
	// �� �޸𸮸� ���� �����ؾ� �ϴ°�?
	bool bToInitializeMapMemory = true;
};