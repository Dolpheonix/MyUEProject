#include "MainGameInstance.h"
#include "../../quickstart.h"
#include "../SaveObject/GameSaver.h"
#include "../GameMode/MainGameMode.h"
#include "../../Object/Interactable/CollectableItem.h"
#include "../../Character/NPC/NPC.h"
#include "../../Utils/JsonHelpers.h"
#include "Kismet/GameplayStatics.h"

UMainGameInstance::UMainGameInstance()
{
}

void UMainGameInstance::Init()
{
	Super::Init();

	// Init() 함수가 여러번 호출되는 경우가 있어, 최초 호출 시에만 바인딩 및 로딩을 진행
	if (!bApplyBound)
	{
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMainGameInstance::LoadNPC);			// 맵 로드시 NPC 데이터를 적용
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMainGameInstance::LoadItems);		// 맵 로드시 루팅 아이템 데이터를 적용
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMainGameInstance::LoadObjects);		// 맵 로드시 다이내믹 오브젝트를 스폰

		JsonLoader::LoadQuests(Quests); // 퀘스트 파일 로딩

		// 서브퀘스트들이 메인 퀘스트의 포인터를 가져야 함. json 형식으로 저장이 불가능해 로딩시 적용
		for (int i = 0; i < Quests.Num(); i++)
		{
			for (int j = 0; j < Quests[i].SubQuests.Num(); j++)
			{
				Quests[i].SubQuests[j].Owner = &Quests[i];
			}
		}

		bApplyBound = true;
	}
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UMainGameInstance::InitializeCharacterMemory(FString Name)
{
	// 캐릭터 관련 기본 데이터 생성
	CharacterMemory.DisplayName = Name;
	CharacterMemory.CurrentMap = "Tutorial"; // 튜토리얼 맵부터 시작
	CharacterMemory.CurrentPos = FVector(0.0f, 0.0f, -100.0f); // 기본 스폰 위치에서 시작
	CharacterMemory.CurrentHP = 100.0f;
	CharacterMemory.CurrentMoney = 10000;

	// 기본 아이템 데이터 생성
	FItemForm fist = FItemForm(FItemShortForm(ETypeTag::Weapon, "Fist")); 
	FItemForm noitem = FItemForm(FItemShortForm(ETypeTag::Item, "NoItem"));
	FItemForm nocloth = FItemForm(FItemShortForm(ETypeTag::Cloth, "NoCloth"));
	fist.ShortForm.InfoTag = "Fist";
	fist.ShortForm.Code = 0;
	fist.ShortForm.bIsSellable = false;
	noitem.ShortForm.InfoTag = " NoItem";
	noitem.ShortForm.bIsSellable = false;
	nocloth.ShortForm.InfoTag = "NoCloth";
	nocloth.ShortForm.bIsSellable = false;
	// 썸네일 로드
	fist.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Fist_Normal.Fist_Normal"));
	fist.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/Fist_Hovered.Fist_Hovered"));
	fist.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/Fist_Selected.Fist_Selected"));
	noitem.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoItem_Normal.NoItem_Normal"));
	noitem.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoItem_Hovered.NoItem_Hovered"));
	noitem.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoItem_Selected.NoItem_Selected"));
	nocloth.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoCloth_Normal.NoCloth_Normal"));
	nocloth.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoCloth_Hovered.NoCloth_Hovered"));
	nocloth.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoCloth_Selected.NoCloth_Selected"));
	// 인벤토리에 추가
	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Weapons
	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Items
	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Clothes
	CharacterMemory.Inventory[uint8(ETypeTag::Weapon)].ItemForms.Add(fist);
	CharacterMemory.Inventory[uint8(ETypeTag::Item)].ItemForms.Add(noitem);
	CharacterMemory.Inventory[uint8(ETypeTag::Cloth)].ItemForms.Add(nocloth);
	//퀵슬롯 설정
	CharacterMemory.Quickslots_Before = TArray<int>({ 0,0,0 });
	CharacterMemory.Quickslots_Now = TArray<int>({ 0,0,0 });
	CharacterMemory.Quickslots_Next = TArray<int>({ 0,0,0 });
}

void UMainGameInstance::ApplyCharacterMemory(AMainCharacter* character)
{
	character->CurrLevel = CharacterMemory.CurrentLevel;
	character->DisplayName = CharacterMemory.DisplayName;
	if (CharacterMemory.CurrentPos.Z > 0)							// Z <= 0이면 초기 스폰 위치에서 시작
	{
		character->SetActorLocation(CharacterMemory.CurrentPos);
		character->SetActorRotation(CharacterMemory.CurrentRot);
	}
	character->HP = CharacterMemory.CurrentHP;
	character->CurrMoney = CharacterMemory.CurrentMoney;
	character->Inventory = CharacterMemory.Inventory;
	character->Quickslots_Before = CharacterMemory.Quickslots_Before;
	character->Quickslots_Now = CharacterMemory.Quickslots_Now;
	character->Quickslots_Next = CharacterMemory.Quickslots_Next;

	character->QuestList = CharacterMemory_Quest;
}

void UMainGameInstance::SaveCharacterMemory(AMainCharacter* character)
{
	CharacterMemory.CurrentLevel = character->CurrLevel;
	CharacterMemory.CurrentMap = UGameplayStatics::GetCurrentLevelName(GetWorld());
	CharacterMemory.CurrentPos = character->GetActorLocation();
	CharacterMemory.CurrentRot = character->GetActorRotation();
	CharacterMemory.CurrentHP = character->HP;
	CharacterMemory.CurrentMoney = character->CurrMoney;
	CharacterMemory.Inventory = character->Inventory;
	CharacterMemory.Quickslots_Before = character->Quickslots_Before;
	CharacterMemory.Quickslots_Now = character->Quickslots_Now;
	CharacterMemory.Quickslots_Next = character->Quickslots_Next;

	CharacterMemory_Quest = character->QuestList;
}

void UMainGameInstance::LoadFromFile(FString SlotName)
{
	// 세이브파일 로드
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectSavedDir() + "SaveGames/" + SlotName + ".json"));
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	FJsonSerializer::Deserialize(Reader, JsonObject);

	SlotIndex = JsonObject->GetIntegerField(TEXT("SlotIndex"));	// 슬롯 인덱스 불러오기
	JsonLoader::LoadCharacterMemory(JsonObject->GetObjectField(TEXT("CharacterMemory")), CharacterMemory); // 캐릭터 메모리 불러오기
	JsonLoader::LoadMapMemories(JsonObject->GetArrayField(TEXT("MapMemories")), MapMemories); // 전체 맵 메모리 불러오기
	ApplyQuestStatus(JsonLoader::LoadQuestStatus(JsonObject->GetArrayField(TEXT("QuestStatus")))); // 퀘스트 진행도 불러오기
}

void UMainGameInstance::ApplyQuestStatus(TArray<FQuestStatus> QuestStatus)
{
	for (int i = 0; i < QuestStatus.Num(); i++)
	{
		FQuestStatus QS = QuestStatus[i];

		if (Quests.Num() <= QS.QuestIndex)
		{
			UE_LOG(LogTemp, Error, TEXT("Quests.Num() <= %d"), QS.QuestIndex);
		}
		else
		{
			FQuest* Quest = &Quests[QS.QuestIndex];
			CharacterMemory_Quest.WorkingQuests.Add(Quest);

			Quest->Progress = QS.Progress;
			Quest->CurrPhase = QS.CurrPhase;
			Quest->Remains = QS.Remains;

			if (Quest->Type == EQuestType::Serial)	// Serial 퀘스트
			{
				for (int j = 0; j < Quest->SubQuests.Num(); j++)
				{
					FSingleQuest* SubQuest = &Quest->SubQuests[j];
					SubQuest->Completed = QS.SubStatus[j].Completed;
					SubQuest->currAmounts = QS.SubStatus[j].CurrAmount;

					if (j <= Quest->CurrPhase)	// 현재 진행중인 subquest까지만 캐릭터에 등록
					{
						if (SubQuest->Type == ESingleQuestType::Arrival)
						{
							CharacterMemory_Quest.ArrivalQuests.Add(SubQuest);
						}
						else if (SubQuest->Type == ESingleQuestType::Item)
						{
							CharacterMemory_Quest.ItemQuests.Add(SubQuest);
						}
						else if (SubQuest->Type == ESingleQuestType::Hunt)
						{
							CharacterMemory_Quest.HuntingQuests.Add(SubQuest);
						}
						else if(SubQuest->Type == ESingleQuestType::Action)
						{
							CharacterMemory_Quest.ActionQuests.Add(SubQuest);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Invalid quest type"));
						}
					}
				}
			}
			else									// Parallel 퀘스트
			{
				for (int j = 0; j < QS.SubStatus.Num(); j++)
				{
					FSingleQuest* SubQuest = &Quest->SubQuests[j];
					SubQuest->Completed = QS.SubStatus[j].Completed;
					SubQuest->currAmounts = QS.SubStatus[j].CurrAmount;

					if (SubQuest->Type == ESingleQuestType::Arrival)
					{
						CharacterMemory_Quest.ArrivalQuests.Add(SubQuest);
					}
					else if (SubQuest->Type == ESingleQuestType::Item)
					{
						CharacterMemory_Quest.ItemQuests.Add(SubQuest);
					}
					else if (SubQuest->Type == ESingleQuestType::Hunt)
					{
						CharacterMemory_Quest.HuntingQuests.Add(SubQuest);
					}
					else if (SubQuest->Type == ESingleQuestType::Action)
					{
						CharacterMemory_Quest.ActionQuests.Add(SubQuest);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Invalid quest type"));
					}
				}
			}
		}
	}
}

bool UMainGameInstance::SaveToFile()
{
	SaveLevel(); // 현재 레벨의 NPC, 아이템 상태를 저장
	TArray<FQuestStatus> QuestStatus = ConvertToQuestStatus(); // 퀘스트 진행도를 json 오브젝트로 변환

	FString FilePath = *(FPaths::ProjectSavedDir() + "SaveGames/" + CharacterMemory.DisplayName + ".json");
	TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
	Obj->SetNumberField(TEXT("Slotindex"), SlotIndex);
	Obj->SetObjectField(TEXT("CharacterMemory"), JsonSaver::SaveCharacterMemory(CharacterMemory)); // 캐릭터 메모리 저장
	TArray<TSharedPtr<FJsonValue>> MMS;
	for (int i = 0; i < MapMemories.Num(); i++)
	{
		MMS.Add(MakeShareable(new FJsonValueObject(JsonSaver::SaveMapMemory(MapMemories[i])))); // 전체 맵 메모리 저장
	}
	Obj->SetArrayField(TEXT("MapMemories"), MMS);
	Obj->SetArrayField(TEXT("QuestStatus"), JsonSaver::SaveQuestStatuses(QuestStatus)); // 퀘스트 진행도 저장

	return JsonSaver::SaveObjectToJson(Obj, FilePath); // 전체 세이브파일 저장
}

TArray<FQuestStatus> UMainGameInstance::ConvertToQuestStatus()
{
	TArray<FQuestStatus> QSS;
	for (int i = 0; i < CharacterMemory_Quest.WorkingQuests.Num(); i++)
	{
		FQuest* Q = CharacterMemory_Quest.WorkingQuests[i];

		FQuestStatus QS;
		QS.QuestIndex = Q->Index;
		QS.Progress = Q->Progress;
		QS.CurrPhase = Q->CurrPhase;
		QS.Remains = Q->Remains;
		for (int j = 0; j < Q->SubQuests.Num(); j++)
		{
			FSingleQuest* SingleQuest = &Q->SubQuests[j];

			FSubQuestStatus SQS;
			SQS.Completed = SingleQuest->Completed;
			SQS.CurrAmount = SingleQuest->currAmounts;

			QS.SubStatus.Add(SQS);
		}

		QSS.Add(QS);
	}

	return QSS;
}

void UMainGameInstance::SaveLevel()
{
	if (CurrentMapMemory)
	{
		SaveNPC();		// NPC 저장
		SaveItems();	// 루팅 아이템 저장
	}
}

void UMainGameInstance::SaveNPC()
{
	TArray<AActor*> NPCList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), NPCList);	// 현재 레벨의 NPC 액터 리스트

	for (int i = 0; i < CurrentMapMemory->LocalNPCMemory.Num(); i++)					// 맵 메모리의 NPC 리스트
	{
		FNPCMemory* NPCMem = &CurrentMapMemory->LocalNPCMemory[i];
		FString ActorName = NPCMem->ActorMemory.Name;
		UE_LOG(LogTemp, Log, TEXT("CurrentMapMemory has %s"), *ActorName);

		AActor** FoundPtr = NPCList.FindByPredicate([ActorName](const AActor* actor) {	// 맵 메모리의 NPC에 상응하는 NPC 액터 리턴
			return actor->GetName() == ActorName;
			});

		if (FoundPtr)
		{
			ANPC* NPC = Cast<ANPC>(*FoundPtr);
			if (NPC)	// 아직 맵에 존재한다면
			{
				NPCMem->ActorMemory.IsValid = true;
				NPCMem->ActorMemory.CurrentPos = NPC->GetActorLocation();
				NPCMem->ActorMemory.CurrentRot = NPC->GetActorRotation();
				NPCMem->DialoguePhase = NPC->DialogueTree.StartIndex;
				SaveHelpers::SaveNPCShopItems(NPC->ShopItems, NPCMem->ShopItems);
			}
		}
		else
		{
			NPCMem->ActorMemory.IsValid = false;	// 맵에서 사라짐 ==> Invalid 상태
		}
	}
}

void UMainGameInstance::SaveItems()
{
	TArray<AActor*> CollectableItemList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectableItem::StaticClass(), CollectableItemList);	// 현재 레벨의 루팅 아이템 리스트

	for (int i = 0; i < CurrentMapMemory->OriginActorMemory_CollectableItem.Num(); i++)
	{
		FActorMemory* ActorMem = &CurrentMapMemory->OriginActorMemory_CollectableItem[i];
		FString ActorName = ActorMem->Name;
		UE_LOG(LogTemp, Log, TEXT("CurrentMapMemory has %s"), *ActorName);
		AActor** FoundPtr = CollectableItemList.FindByPredicate([ActorName](const AActor* actor) {
			return actor->GetName() == ActorName;
			});

		if (FoundPtr)
		{
			UE_LOG(LogTemp, Log, TEXT("%s is valid"), *ActorName);
			ActorMem->IsValid = true;
			ActorMem->CurrentPos = (*FoundPtr)->GetActorLocation();
			ActorMem->CurrentRot = (*FoundPtr)->GetActorRotation();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s is invalid"), *ActorName);
			ActorMem->IsValid = false;
		}
	}
}

void UMainGameInstance::Start()
{
	LoadLevel(CharacterMemory.CurrentMap, true);
}

void UMainGameInstance::LoadLevel(FString MapName, bool isStart)
{
	if (!isStart)	// 게임 시작이 아님 ==> 게임 플레이 중 맵 전환이 발생
	{
		SaveCharacterMemory(Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)));	// 맵 전환 전에 캐릭터 정보를 임시저장
		SaveToFile();	// 파일에 자동저장
		CharacterMemory.CurrentPos = FVector(0.0f, 0.0f, -1.0f);	// Z < 0 : 기본 위치에 스폰
	}

	if (MapName != "Intro_Empty")	// 인트로 맵이 아닌 경우(일반적인 게임 상황)
	{
		CharacterMemory.CurrentMap = MapName;
		
		CurrentMapMemory = MapMemories.FindByPredicate([MapName](const FMapMemory MM) {	// 불러올 맵의 메모리
			return MM.Name == MapName;
			});

		if (!CurrentMapMemory)	// 맵 메모리가 존재하지 않음(처음 불러오는 맵)
		{
			FMapMemory newMM;
			newMM.Name = MapName;
			MapMemories.Add(newMM);

			CurrentMapMemory = &MapMemories[MapMemories.Num() - 1];
			bToInitializeMapMemory = true;
		}
		else
		{
			bToInitializeMapMemory = false;
		}
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName));
}

int32 UMainGameInstance::AllocateSlotIndex()
{
	TArray<FString> SlotList;
	if (JsonLoader::LoadSlotList(SlotList))	// 슬롯 정보를 저장한 리스트
	{
		for (int i = 0; i < MAX_SAVE_SLOTS; i++)
		{
			if (SlotList[i] == "")	// 빈자리 발견
			{
				SlotList[i] = CharacterMemory.DisplayName;
				SlotIndex = i;

				JsonSaver::SaveSlotList(SlotList);
				return i;
			}
			else if (SlotList[i] == CharacterMemory.DisplayName)	// 이미 존재하는 이름
			{
				return -1;
			}
		}

		return -2;	// 슬롯이 꽉참
	}
	else // 슬롯 리스트가 존재하지 않음 ==> 새로 생성 후 0번에 할당
	{
		SlotList.Empty();
		SlotList.SetNum(MAX_SAVE_SLOTS);
		for (int i = 0; i < MAX_SAVE_SLOTS; i++) SlotList[i] = "";
		SlotList[0] = CharacterMemory.DisplayName;
		JsonSaver::SaveSlotList(SlotList);
		return 0;
	}
}

void UMainGameInstance::LoadNPC(UWorld* world)
{
	TArray<AActor*> NPCList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), NPCList);	// 현재 레벨의 NPC 리스트

	JsonLoader::LoadNPCDialogue(NPCList);	// 대화 데이터를 NPC 액터에 적용

	if (bToInitializeMapMemory)	// 맵 메모리를 초기화하지 않음 ==> 현재 NPC 리스트 모두 메모리에 추가
	{
		for (int i = 0; i < NPCList.Num(); i++)
		{
			ANPC* NPC = Cast<ANPC>(NPCList[i]);
			if (NPC)
			{
				FNPCMemory NPCMem;
				NPCMem.DisplayName = NPC->DisplayName;
				NPCMem.ActorMemory.Name = NPC->GetName();
				NPCMem.ActorMemory.IsValid = true;
				NPCMem.DialoguePhase = 0;
				CurrentMapMemory->LocalNPCMemory.Add(NPCMem);
			}
		}
	}
	else
	{
		for (FNPCMemory NPCMem : CurrentMapMemory->LocalNPCMemory)
		{
			FString NPCName = NPCMem.ActorMemory.Name;
			AActor** FoundPtr = NPCList.FindByPredicate([NPCName](const AActor* actor) {	// NPC 메모리에 상응하는 NPC 액터
				return actor->GetName() == NPCName;
				});

			if (FoundPtr)
			{
				if (NPCMem.ActorMemory.IsValid)	// 아직 존재하는 NPC
				{
					ANPC* NPC = Cast<ANPC>(*FoundPtr);
					NPC->DialogueTree.StartIndex = NPCMem.DialoguePhase;
					NPC->ShopItemsInfo = NPCMem.ShopItems;
					NPC->GenerateShopItems();
					NPC->SetActorLocationAndRotation(NPCMem.ActorMemory.CurrentPos, NPCMem.ActorMemory.CurrentRot);
				}
				else // Invalid NPC ==> 삭제
				{
					(*FoundPtr)->Destroy();
				}
			}
		}
	}
}

void UMainGameInstance::LoadItems(UWorld* world)
{
	TArray<AActor*> CollectableItemList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectableItem::StaticClass(), CollectableItemList);	// 현재 레벨의 루팅 아이템 리스트

	if (bToInitializeMapMemory)	// 루팅 아이템 전부 메모리에 추가
	{
		for (AActor* ActorPtr : CollectableItemList)
		{
			FActorMemory ActorMem;
			ActorMem.Name = ActorPtr->GetName();
			ActorMem.IsValid = true;
			CurrentMapMemory->OriginActorMemory_CollectableItem.Add(ActorMem);
		}
	}
	else
	{
		for (FActorMemory ActorMem : CurrentMapMemory->OriginActorMemory_CollectableItem)
		{
			FString ActorName = ActorMem.Name;
			AActor** FoundPtr = CollectableItemList.FindByPredicate([ActorName](const AActor* actor) {
				return actor->GetName() == ActorName;
				});

			if (FoundPtr)
			{
				if (ActorMem.IsValid)
				{
					(*FoundPtr)->SetActorLocationAndRotation(ActorMem.CurrentPos, ActorMem.CurrentRot);
				}
				else
				{
					(*FoundPtr)->Destroy();
				}
			}
		}
	}
}

void UMainGameInstance::LoadObjects(UWorld* world)
{
	// case 0 : 도착 퀘스트의 도착지 Indicator
	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Player)
	{
		for (int i = 0; i < Player->QuestList.ArrivalQuests.Num(); i++)	// 플레이어가 진행중인 도착 퀘스트
		{
			FSingleQuest* sq = Player->QuestList.ArrivalQuests[i];
			if (sq && sq->MapName == CurrentMapMemory->Name && !sq->Completed)
			{
				Player->RegisterDestinationFlagVolume(sq);	// 도착지 비콘 액터 스폰
			}
		}
	}
}

FQuest* UMainGameInstance::GetQuest(int index)
{
	if (Quests.Num() <= index)
	{
		UE_LOG(LogTemp, Error, TEXT("Quests.Num() <= %d"), index);
		return nullptr;
	}
	else
	{
		return &Quests[index];
	}
}