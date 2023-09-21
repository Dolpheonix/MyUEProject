/////////////UMainGameInstance/////////////////////
// 게임 모드와 함께 게임 전반을 관리하는 게임 인스턴스입니다.
// 1. 세이브 파일 관리 기능
// 2. 레벨 전환 시 캐릭터의 정보 임시 저장
// 3. 퀘스트 로딩 및 저장 기능

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
	
	// 게임 시작시 호출
	virtual void Init() override;
	// 게임 종료시 호출 
	virtual void Shutdown() override;

public:
	// 캐릭터 관련 데이터를 초기화
	void InitializeCharacterMemory(FString Name);
	// 불러온(초기화한) 캐릭터 데이터를 스폰된 액터에 적용 
	void ApplyCharacterMemory(AMainCharacter* character);
	// 캐릭터의 데이터를 게임인스턴트에 임시저장
	void SaveCharacterMemory(AMainCharacter* character);

	// 게임인스턴스의 게임 데이터를 세이브파일에 저장
	bool SaveToFile();
	// 퀘스트 상태를 FQuestStatus 구조체 형식으로 변환
	TArray<FQuestStatus> ConvertToQuestStatus();
	// 맵 상태를 저장
	void SaveLevel();
	// 맵의 NPC 상태를 저장
	void SaveNPC();
	// 맵의 루팅 아이템 상태를 저장
	void SaveItems();

	// 파일로부터 게임 데이터를 불러옴
	void LoadFromFile(FString Slotname);
	// 퀘스트 진행 상황을 퀘스트 오브젝트에 적용
	// QuestStatus : 파일로부터 불러온 퀘스트 진행상황
	void ApplyQuestStatus(TArray<FQuestStatus> QuestStatus);
	// 맵 데이터와 함께 맵을 불러옴
	UFUNCTION(BlueprintCallable)
	void LoadLevel(FString MapName, bool isStart);
	// NPC 상태를 맵에 적용
	UFUNCTION()
	void LoadNPC(UWorld* world);
	// 루팅 아이템 상태를 맵에 적용
	UFUNCTION()
	void LoadItems(UWorld* world);
	// 그 외의 다이내믹 오브젝트를 맵에 적용(EX. 도착 퀘스트의 도착지점 표시 FX)
	UFUNCTION()
	void LoadObjects(UWorld* world);

	// 게임을 시작함
	void Start();

	// 세이브파일을 새로 생성 시, 배정받을 인덱스를 리턴
	int32 AllocateSlotIndex();

	// 인덱스에 맞는 퀘스트 리턴
	FQuest* GetQuest(int index);

public:
	// 퀘스트의 정보 및 상태를 저장(게임플레이 동안)
	UPROPERTY(BlueprintReadWrite)
	TArray<FQuest> Quests;
	// 캐릭터 관련 데이터 (레벨 전환시 임시저장용)
	UPROPERTY(BlueprintReadWrite)
	FCharacterMemory CharacterMemory;
	// 플레이어가 현재 진행중인 퀘스트 목록 (레벨 전환시 임시저장용)
	UPROPERTY(BlueprintReadWrite)
	FWorkingQuestMemory CharacterMemory_Quest;
	// 전체 맵 데이터 (레벨 로드시 맵에 적용)
	UPROPERTY(BlueprintReadWrite)
	TArray<FMapMemory> MapMemories;
	// 현재 맵의 데이터를 참조
	FMapMemory* CurrentMapMemory;
	// 현재 플레이어의 세이브 슬롯 번호
	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex;

	// Init() 함수 최초 호출 시점 확인
	bool bApplyBound = false;
	// 맵 메모리를 새로 생성해야 하는가?
	bool bToInitializeMapMemory = true;
};