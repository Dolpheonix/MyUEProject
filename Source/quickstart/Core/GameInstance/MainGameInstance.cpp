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

	// Init() �Լ��� ������ ȣ��Ǵ� ��찡 �־�, ���� ȣ�� �ÿ��� ���ε� �� �ε��� ����
	if (!bApplyBound)
	{
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMainGameInstance::LoadNPC);			// �� �ε�� NPC �����͸� ����
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMainGameInstance::LoadItems);		// �� �ε�� ���� ������ �����͸� ����
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMainGameInstance::LoadObjects);		// �� �ε�� ���̳��� ������Ʈ�� ����

		JsonLoader::LoadQuests(Quests); // ����Ʈ ���� �ε�

		// ��������Ʈ���� ���� ����Ʈ�� �����͸� ������ ��. json �������� ������ �Ұ����� �ε��� ����
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
	// ĳ���� ���� �⺻ ������ ����
	CharacterMemory.DisplayName = Name;
	CharacterMemory.CurrentMap = "Tutorial"; // Ʃ�丮�� �ʺ��� ����
	CharacterMemory.CurrentPos = FVector(0.0f, 0.0f, -100.0f); // �⺻ ���� ��ġ���� ����
	CharacterMemory.CurrentHP = 100.0f;
	CharacterMemory.CurrentMoney = 10000;

	// �⺻ ������ ������ ����
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
	// ����� �ε�
	fist.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Fist_Normal.Fist_Normal"));
	fist.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/Fist_Hovered.Fist_Hovered"));
	fist.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/Fist_Selected.Fist_Selected"));
	noitem.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoItem_Normal.NoItem_Normal"));
	noitem.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoItem_Hovered.NoItem_Hovered"));
	noitem.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoItem_Selected.NoItem_Selected"));
	nocloth.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoCloth_Normal.NoCloth_Normal"));
	nocloth.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoCloth_Hovered.NoCloth_Hovered"));
	nocloth.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoCloth_Selected.NoCloth_Selected"));
	// �κ��丮�� �߰�
	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Weapons
	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Items
	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Clothes
	CharacterMemory.Inventory[uint8(ETypeTag::Weapon)].ItemForms.Add(fist);
	CharacterMemory.Inventory[uint8(ETypeTag::Item)].ItemForms.Add(noitem);
	CharacterMemory.Inventory[uint8(ETypeTag::Cloth)].ItemForms.Add(nocloth);
	//������ ����
	CharacterMemory.Quickslots_Before = TArray<int>({ 0,0,0 });
	CharacterMemory.Quickslots_Now = TArray<int>({ 0,0,0 });
	CharacterMemory.Quickslots_Next = TArray<int>({ 0,0,0 });
}

void UMainGameInstance::ApplyCharacterMemory(AMainCharacter* character)
{
	character->CurrLevel = CharacterMemory.CurrentLevel;
	character->DisplayName = CharacterMemory.DisplayName;
	if (CharacterMemory.CurrentPos.Z > 0)							// Z <= 0�̸� �ʱ� ���� ��ġ���� ����
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
	// ���̺����� �ε�
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectSavedDir() + "SaveGames/" + SlotName + ".json"));
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	FJsonSerializer::Deserialize(Reader, JsonObject);

	SlotIndex = JsonObject->GetIntegerField(TEXT("SlotIndex"));	// ���� �ε��� �ҷ�����
	JsonLoader::LoadCharacterMemory(JsonObject->GetObjectField(TEXT("CharacterMemory")), CharacterMemory); // ĳ���� �޸� �ҷ�����
	JsonLoader::LoadMapMemories(JsonObject->GetArrayField(TEXT("MapMemories")), MapMemories); // ��ü �� �޸� �ҷ�����
	ApplyQuestStatus(JsonLoader::LoadQuestStatus(JsonObject->GetArrayField(TEXT("QuestStatus")))); // ����Ʈ ���൵ �ҷ�����
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

			if (Quest->Type == EQuestType::Serial)	// Serial ����Ʈ
			{
				for (int j = 0; j < Quest->SubQuests.Num(); j++)
				{
					FSingleQuest* SubQuest = &Quest->SubQuests[j];
					SubQuest->Completed = QS.SubStatus[j].Completed;
					SubQuest->currAmounts = QS.SubStatus[j].CurrAmount;

					if (j <= Quest->CurrPhase)	// ���� �������� subquest������ ĳ���Ϳ� ���
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
			else									// Parallel ����Ʈ
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
	SaveLevel(); // ���� ������ NPC, ������ ���¸� ����
	TArray<FQuestStatus> QuestStatus = ConvertToQuestStatus(); // ����Ʈ ���൵�� json ������Ʈ�� ��ȯ

	FString FilePath = *(FPaths::ProjectSavedDir() + "SaveGames/" + CharacterMemory.DisplayName + ".json");
	TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
	Obj->SetNumberField(TEXT("Slotindex"), SlotIndex);
	Obj->SetObjectField(TEXT("CharacterMemory"), JsonSaver::SaveCharacterMemory(CharacterMemory)); // ĳ���� �޸� ����
	TArray<TSharedPtr<FJsonValue>> MMS;
	for (int i = 0; i < MapMemories.Num(); i++)
	{
		MMS.Add(MakeShareable(new FJsonValueObject(JsonSaver::SaveMapMemory(MapMemories[i])))); // ��ü �� �޸� ����
	}
	Obj->SetArrayField(TEXT("MapMemories"), MMS);
	Obj->SetArrayField(TEXT("QuestStatus"), JsonSaver::SaveQuestStatuses(QuestStatus)); // ����Ʈ ���൵ ����

	return JsonSaver::SaveObjectToJson(Obj, FilePath); // ��ü ���̺����� ����
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
		SaveNPC();		// NPC ����
		SaveItems();	// ���� ������ ����
	}
}

void UMainGameInstance::SaveNPC()
{
	TArray<AActor*> NPCList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), NPCList);	// ���� ������ NPC ���� ����Ʈ

	for (int i = 0; i < CurrentMapMemory->LocalNPCMemory.Num(); i++)					// �� �޸��� NPC ����Ʈ
	{
		FNPCMemory* NPCMem = &CurrentMapMemory->LocalNPCMemory[i];
		FString ActorName = NPCMem->ActorMemory.Name;
		UE_LOG(LogTemp, Log, TEXT("CurrentMapMemory has %s"), *ActorName);

		AActor** FoundPtr = NPCList.FindByPredicate([ActorName](const AActor* actor) {	// �� �޸��� NPC�� �����ϴ� NPC ���� ����
			return actor->GetName() == ActorName;
			});

		if (FoundPtr)
		{
			ANPC* NPC = Cast<ANPC>(*FoundPtr);
			if (NPC)	// ���� �ʿ� �����Ѵٸ�
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
			NPCMem->ActorMemory.IsValid = false;	// �ʿ��� ����� ==> Invalid ����
		}
	}
}

void UMainGameInstance::SaveItems()
{
	TArray<AActor*> CollectableItemList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectableItem::StaticClass(), CollectableItemList);	// ���� ������ ���� ������ ����Ʈ

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
	if (!isStart)	// ���� ������ �ƴ� ==> ���� �÷��� �� �� ��ȯ�� �߻�
	{
		SaveCharacterMemory(Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)));	// �� ��ȯ ���� ĳ���� ������ �ӽ�����
		SaveToFile();	// ���Ͽ� �ڵ�����
		CharacterMemory.CurrentPos = FVector(0.0f, 0.0f, -1.0f);	// Z < 0 : �⺻ ��ġ�� ����
	}

	if (MapName != "Intro_Empty")	// ��Ʈ�� ���� �ƴ� ���(�Ϲ����� ���� ��Ȳ)
	{
		CharacterMemory.CurrentMap = MapName;
		
		CurrentMapMemory = MapMemories.FindByPredicate([MapName](const FMapMemory MM) {	// �ҷ��� ���� �޸�
			return MM.Name == MapName;
			});

		if (!CurrentMapMemory)	// �� �޸𸮰� �������� ����(ó�� �ҷ����� ��)
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
	if (JsonLoader::LoadSlotList(SlotList))	// ���� ������ ������ ����Ʈ
	{
		for (int i = 0; i < MAX_SAVE_SLOTS; i++)
		{
			if (SlotList[i] == "")	// ���ڸ� �߰�
			{
				SlotList[i] = CharacterMemory.DisplayName;
				SlotIndex = i;

				JsonSaver::SaveSlotList(SlotList);
				return i;
			}
			else if (SlotList[i] == CharacterMemory.DisplayName)	// �̹� �����ϴ� �̸�
			{
				return -1;
			}
		}

		return -2;	// ������ ����
	}
	else // ���� ����Ʈ�� �������� ���� ==> ���� ���� �� 0���� �Ҵ�
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), NPCList);	// ���� ������ NPC ����Ʈ

	JsonLoader::LoadNPCDialogue(NPCList);	// ��ȭ �����͸� NPC ���Ϳ� ����

	if (bToInitializeMapMemory)	// �� �޸𸮸� �ʱ�ȭ���� ���� ==> ���� NPC ����Ʈ ��� �޸𸮿� �߰�
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
			AActor** FoundPtr = NPCList.FindByPredicate([NPCName](const AActor* actor) {	// NPC �޸𸮿� �����ϴ� NPC ����
				return actor->GetName() == NPCName;
				});

			if (FoundPtr)
			{
				if (NPCMem.ActorMemory.IsValid)	// ���� �����ϴ� NPC
				{
					ANPC* NPC = Cast<ANPC>(*FoundPtr);
					NPC->DialogueTree.StartIndex = NPCMem.DialoguePhase;
					NPC->ShopItemsInfo = NPCMem.ShopItems;
					NPC->GenerateShopItems();
					NPC->SetActorLocationAndRotation(NPCMem.ActorMemory.CurrentPos, NPCMem.ActorMemory.CurrentRot);
				}
				else // Invalid NPC ==> ����
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectableItem::StaticClass(), CollectableItemList);	// ���� ������ ���� ������ ����Ʈ

	if (bToInitializeMapMemory)	// ���� ������ ���� �޸𸮿� �߰�
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
	// case 0 : ���� ����Ʈ�� ������ Indicator
	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Player)
	{
		for (int i = 0; i < Player->QuestList.ArrivalQuests.Num(); i++)	// �÷��̾ �������� ���� ����Ʈ
		{
			FSingleQuest* sq = Player->QuestList.ArrivalQuests[i];
			if (sq && sq->MapName == CurrentMapMemory->Name && !sq->Completed)
			{
				Player->RegisterDestinationFlagVolume(sq);	// ������ ���� ���� ����
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