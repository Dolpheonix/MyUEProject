#pragma once
#include "CoreMinimal.h"
#include "Structs.h"
#include "../Core/SaveObject/GameSaver.h"
#include "../Character/NPC/NPC.h"
#include "UObject/Object.h"

// Json 파일을 구조체에 로드
class JsonLoader
{
public:
	// 문자열을 ETypeTag로 변환
	static ETypeTag EnumifyItemType(FString TypeStr)
	{
		ETypeTag OutVal;

		if (TypeStr == "Weapon")
		{
			OutVal = ETypeTag::Weapon;
		}
		else if (TypeStr == "Item")
		{
			OutVal = ETypeTag::Item;
		}
		else if (TypeStr == "Cloth")
		{
			OutVal = ETypeTag::Cloth;
		}
		else
		{
			UE_LOG(LogJson, Log, TEXT("ETypeTag::%s is not valid"), *TypeStr);
		}

		return OutVal;
	}
	// 문자열을 EQuestProgress로 변환
	static EQuestProgress EnumifyQuestProgressType(FString TypeStr)
	{
		if (TypeStr == "Unavailable")
		{
			return EQuestProgress::Unavailable;
		}
		else if (TypeStr == "Available")
		{
			return EQuestProgress::Available;
		}
		else if (TypeStr == "InProgress")
		{
			return EQuestProgress::InProgress;
		}
		else if (TypeStr == "Finished")
		{
			return EQuestProgress::Finished;
		}
		else if (TypeStr == "AlreadyDone")
		{
			return EQuestProgress::AlreadyDone;
		}
		else
		{
			UE_LOG(LogJson, Error, TEXT("EQuestProgress::%s is not valid"), *TypeStr);
			return EQuestProgress::Max;
		}
	}
	// Json 배열을 벡터로 변환
	static FVector LoadVector(TArray<TSharedPtr<FJsonValue>> Vec)
	{
		FVector OutVal;
		OutVal.X = Vec[0]->AsNumber();
		OutVal.Y = Vec[1]->AsNumber();
		OutVal.Z = Vec[2]->AsNumber();

		return OutVal;
	}
	// Json 오브젝트를 FRotator로 변환
	static FRotator LoadRotator(TSharedPtr<FJsonObject> Rot)
	{
		FRotator OutVal;
		OutVal.Yaw = Rot->GetNumberField(TEXT("Yaw"));
		OutVal.Pitch = Rot->GetNumberField(TEXT("Pitch"));
		OutVal.Roll = Rot->GetNumberField(TEXT("Roll"));

		return OutVal;
	}
	// Json 오브젝트를 FDialogueEvent로 변환
	static void LoadDialogueResponseEvents(TArray<TSharedPtr<FJsonValue>> Events, TArray<FDialogueEvent>& OutVal)
	{
		for (int i = 0; i < Events.Num(); i++)
		{
			TSharedPtr<FJsonObject> Event = Events[i]->AsObject();

			FDialogueEvent Eve;

			FString TypeStr = Event->GetStringField(TEXT("Type"));
			if (TypeStr == "OpenQuest")
			{
				Eve.EventType = EDialogueEventType::OPENQUEST;
				Eve.QuestIndex = Event->GetIntegerField(TEXT("QuestIndex"));
			}
			else if (TypeStr == "GiveItem")
			{
				Eve.EventType = EDialogueEventType::GIVEITEM;
				Eve.ItemIndex = Event->GetIntegerField(TEXT("ItemIndex"));
			}
			else if (TypeStr == "PHASESHIFT")
			{
				Eve.EventType = EDialogueEventType::PHASESHIFT;
				Eve.NextPhaseIndex = Event->GetIntegerField(TEXT("NextPhase"));
			}
			else if (TypeStr == "OpenShop")
			{
				Eve.EventType = EDialogueEventType::OPENSHOP;
			}
			else
			{
				UE_LOG(LogJson, Log, TEXT("EDialogueEventType::%s is not valid"), *TypeStr);
			}

			OutVal.Add(Eve);
		}
	}
	// Json 오브젝트를 FDialogueResponse로 변환
	static void LoadDialogueResponses(TArray<TSharedPtr<FJsonValue>> Responses, TArray<FDialogueResponse>& OutVal)
	{
		for (int i = 0; i < Responses.Num(); i++)
		{
			TSharedPtr<FJsonObject> Response = Responses[i]->AsObject();

			FDialogueResponse Res;
			Res.Response = Response->GetStringField(TEXT("Response"));
			Res.IsEnd = Response->GetBoolField(TEXT("IsEnd"));
			Res.NextIndex = Response->GetIntegerField(TEXT("Next"));

			TArray<TSharedPtr<FJsonValue>> Events = Response->GetArrayField(TEXT("Events"));
			LoadDialogueResponseEvents(Events, Res.Events);

			OutVal.Add(Res);
		}
	}
	// NPC의 Dialogue를 Json 파일로부터 로드
	static void LoadNPCDialogue(TArray<AActor*>& NPCList)
	{
		for (int i = 0; i < NPCList.Num(); i++)
		{
			ANPC* npc = Cast<ANPC>(NPCList[i]);
			if (npc)
			{
				// 해당 NPC의 이름이 파일 제목인 json 파일을 찾음
				FString DialogueJstr;
				FString DialoguePath = FPaths::ProjectSavedDir() + "Core/Dialogue/" + npc->DisplayName + ".json";
				if (FFileHelper::LoadFileToString(DialogueJstr, *DialoguePath))
				{
					TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(DialogueJstr);
					TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
					FJsonSerializer::Deserialize(JsonReader, JsonObject);

					TArray<TSharedPtr<FJsonValue>> Dialogues = JsonObject->GetArrayField(TEXT("Dialogues"));
					// Dialogue line을 로드
					for (int j = 0; j < Dialogues.Num(); j++)
					{
						TSharedPtr<FJsonObject> Dialogue = Dialogues[j]->AsObject();

						FDialogueLine Dl;
						Dl.Speaker = Dialogue->GetStringField(TEXT("Speaker"));
						Dl.TextLine = Dialogue->GetStringField(TEXT("Text"));

						TArray<TSharedPtr<FJsonValue>> Responses = Dialogue->GetArrayField(TEXT("Responses"));
						LoadDialogueResponses(Responses, Dl.Responses);

						npc->DialogueTree.DialogueLines.Add(Dl);
					}
				}
			}
		}
	}
	// Json 오브젝트를 FItemShortForm으로 변환
	static FItemShortForm LoadItemShortForm(TSharedPtr<FJsonObject> Item)
	{
		FItemShortForm OutVal;

		OutVal.TypeTag = EnumifyItemType(Item->GetStringField(TEXT("Type")));
		OutVal.Code = Item->GetIntegerField(TEXT("Code"));
		OutVal.NameTag = Item->GetStringField(TEXT("Name"));
		OutVal.Num = Item->GetIntegerField(TEXT("Num"));
		OutVal.bIsSellable = Item->GetBoolField(TEXT("Sellable"));

		return OutVal;
	}
	// Json 배열을 FItemShortForm 배열로 변환
	static TArray<FItemShortForm> LoadItemShortForms(TArray<TSharedPtr<FJsonValue>> Items)
	{
		TArray<FItemShortForm> OutVal;

		for (int i = 0; i < Items.Num(); i++)
		{
			OutVal.Add(LoadItemShortForm(Items[i]->AsObject()));
		}

		return OutVal;
	}
	// Json 오브젝트를 FReward로 변환
	static void LoadQuestRewards(TArray<TSharedPtr<FJsonValue>> Rewards, TArray<FReward>& OutVal)
	{
		for (int i = 0; i < Rewards.Num(); i++)
		{
			TSharedPtr<FJsonObject> Reward = Rewards[i]->AsObject();

			FReward Rwd;
			
			FString RewardType = Reward->GetStringField(TEXT("Type"));
			if (RewardType == "EXP")
			{
				Rwd.Type = ERewardType::EXP;
				Rwd.EXP = Reward->GetIntegerField(TEXT("EXP"));
			}
			else if (RewardType == "Money")
			{
				Rwd.Type = ERewardType::MONEY;
				Rwd.Money = Reward->GetIntegerField(TEXT("Money"));
			}
			else if(RewardType == "Item")
			{
				Rwd.Type = ERewardType::ITEM;
				Rwd.Item = LoadItemShortForm(Reward->GetObjectField(TEXT("Item")));
			}
			else
			{
				UE_LOG(LogJson, Log, TEXT("ERewardType::%s is not valid"), *RewardType);
			}
		}
	}
	// Json 오브젝트를 FSingleQuest로 변환
	static void LoadSubQuests(TArray<TSharedPtr<FJsonValue>> SubQuests, TArray<FSingleQuest>& OutVal)
	{
		for (int i = 0; i < SubQuests.Num(); i++)
		{
			TSharedPtr<FJsonObject> SubQuest = SubQuests[i]->AsObject();

			FSingleQuest SQ;
			SQ.Name = SubQuest->GetStringField(TEXT("Name"));

			// 서브퀘스트 타입에 따라 다른 Json Field를 로드
			FString TypeStr = SubQuest->GetStringField(TEXT("Type"));
			if (TypeStr == "Arrival")
			{
				SQ.Type = ESingleQuestType::Arrival;

				TSharedPtr<FJsonObject> Q_Arrival = SubQuest->GetObjectField(TEXT("Q_Arrival"));

				SQ.MapName = Q_Arrival->GetStringField(TEXT("Map"));

				TArray<TSharedPtr<FJsonValue>> Destination = Q_Arrival->GetArrayField(TEXT("Destination"));
				SQ.Destination.X = Destination[0]->AsNumber();
				SQ.Destination.Y = Destination[1]->AsNumber();
				SQ.Destination.Z = Destination[2]->AsNumber();

				SQ.FXTemplate = Helpers::LoadObjectFromPath<UParticleSystem>(TEXT("/Game/ShootingGame/Particle/FX_VarietyPack/FX/P_ky_healAura.P_ky_healAura"));
			}
			else if (TypeStr == "Hunt")
			{
				SQ.Type = ESingleQuestType::Hunt;

				TSharedPtr<FJsonObject> Q_Hunt = SubQuest->GetObjectField(TEXT("Q_Hunt"));

				TArray<TSharedPtr<FJsonValue>> HuntingList = Q_Hunt->GetArrayField(TEXT("List"));
				for (int j = 0; j < HuntingList.Num(); j++)
				{
					TSharedPtr<FJsonObject> HuntForm = HuntingList[j]->AsObject();

					FHuntingQuestForm HQ;
					HQ.Huntee = HuntForm->GetStringField(TEXT("Huntee"));
					HQ.HuntAmount = HuntForm->GetIntegerField(TEXT("Amount"));

					SQ.HuntingLists.Add(HQ);
				}
			}
			else if (TypeStr == "Item")
			{
				SQ.Type = ESingleQuestType::Item;

				TSharedPtr<FJsonObject> Q_Item = SubQuest->GetObjectField(TEXT("Q_Item"));

				TArray<TSharedPtr<FJsonValue>> ItemList = Q_Item->GetArrayField(TEXT("List"));
				for (int j = 0; j < ItemList.Num(); j++)
				{
					TSharedPtr<FJsonObject> ItemForm = ItemList[j]->AsObject();
					
					FItemQuestForm IQ;
					
					IQ.ItemType = EnumifyItemType(ItemForm->GetStringField("Type"));
					IQ.ItemName = ItemForm->GetStringField("Name");
					IQ.ItemAmount = ItemForm->GetIntegerField("Amount");
					
					SQ.ItemLists.Add(IQ);
				}
			}
			else if (TypeStr == "Action")
			{
				SQ.Type = ESingleQuestType::Action;

				TSharedPtr<FJsonObject> Q_Action = SubQuest->GetObjectField(TEXT("Q_Action"));

				SQ.ActionCode = Q_Action->GetIntegerField(TEXT("Code"));
				SQ.ActionInfo = Q_Action->GetStringField(TEXT("Info"));
			}
			else
			{
				UE_LOG(LogJson, Log, TEXT("ESingleQuestType::%s is not valid"), *TypeStr);
			}

			OutVal.Add(SQ);
		}
	}
	// Json 오브젝트를 FQuestDIalogueEvent로 변환
	static void LoadQuestDialogueEvents(TArray<TSharedPtr<FJsonValue>> Events, TArray<FQuestDialogueEvent>& OutVal)
	{
		for (int i = 0; i < Events.Num(); i++)
		{
			TSharedPtr<FJsonObject> Event = Events[i]->AsObject();

			FQuestDialogueEvent DE;
			
			// 이벤트 타입에 따라 다른 필드를 로드
			FString TypeStr = Event->GetStringField(TEXT("Type"));
			if (TypeStr == "Commit")
			{
				DE.EventType = EQuestDialogueEventType::COMMIT;
			}
			else if (TypeStr == "Complete")
			{
				DE.EventType = EQuestDialogueEventType::COMPLETE;
			}
			else if (TypeStr == "GiveItem")
			{
				DE.EventType = EQuestDialogueEventType::GIVEITEM;
				DE.ItemIndex = Event->GetIntegerField(TEXT("Item"));
			}
			else if (TypeStr == "BackToDialogue")
			{
				DE.EventType = EQuestDialogueEventType::BACKTODIALOGUE;
				DE.BacktoDialogueIndex = Event->GetIntegerField(TEXT("BackToNPC"));
			}
			else if (TypeStr == "PhaseShift")
			{
				DE.EventType = EQuestDialogueEventType::PHASESHIFT;
				DE.NextPhaseIndex = Event->GetIntegerField(TEXT("NextPhase"));
			}
			else
			{
				UE_LOG(LogJson, Log, TEXT("EQuestDialogueEventType::%s is not valid"), *TypeStr);
			}

			OutVal.Add(DE);
		}
	}
	// Json 오브젝트를 FQuestDialogueResponse로 변환
	static void LoadQuestDialogueResponses(TArray<TSharedPtr<FJsonValue>> Responses, TArray<FQuestDialogueResponse>& OutVal)
	{
		for (int i = 0; i < Responses.Num(); i++)
		{
			TSharedPtr<FJsonObject> Response = Responses[i]->AsObject();

			FQuestDialogueResponse DR;
			DR.Response = Response->GetStringField(TEXT("Response"));
			DR.IsEnd = Response->GetBoolField(TEXT("IsEnd"));
			DR.NextIndex = Response->GetIntegerField(TEXT("Next"));
			LoadQuestDialogueEvents(Response->GetArrayField(TEXT("Events")), DR.Events);

			OutVal.Add(DR);
		}
	}
	// Json 오브젝트를 FQuestDialogueTree로 변환
	static void LoadQuestDialogues(TArray<TSharedPtr<FJsonValue>> Dialogues, FQuestDialogueTree& OutVal)
	{
		for (int i = 0; i < Dialogues.Num(); i++)
		{
			TSharedPtr<FJsonObject> Dialogue = Dialogues[i]->AsObject();
			
			FQuestDialogueLine DL;
			DL.Speaker = Dialogue->GetStringField(TEXT("Speaker"));
			DL.TextLine = Dialogue->GetStringField(TEXT("Text"));
			LoadQuestDialogueResponses(Dialogue->GetArrayField("Responses"), DL.Responses);

			OutVal.DialogueLines.Add(DL);
		}
	}
	// Json 오브젝트를 FQuest로 변환
	static void LoadQuests(TArray<FQuest>& OutVal)
	{
		// Quest.json 파일을 로드
		FString JsonString;
		FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectSavedDir() + "Core/Quest.json"));
		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(Reader, JsonObject);

		TArray<TSharedPtr<FJsonValue>> Quests = JsonObject->GetArrayField(TEXT("Quests"));

		// FQuest로 변환 후, 배열에 추가
		for (int i = 0; i < Quests.Num(); i++)
		{
			TSharedPtr<FJsonObject> Quest = Quests[i]->AsObject();

			FQuest Q;
			Q.Index = Quest->GetIntegerField(TEXT("Index"));
			Q.Name = Quest->GetStringField(TEXT("Name"));
			Q.Info = Quest->GetStringField(TEXT("Explanation"));
			
			// EQuestType
			FString QuestType = Quest->GetStringField(TEXT("Type"));
			if (QuestType == "Serial")
			{
				Q.Type = EQuestType::Serial;
			}
			else if (QuestType == "Parallel")
			{
				Q.Type = EQuestType::Parallel;
			}
			else
			{
				UE_LOG(LogJson, Log, TEXT("EQuestType::%s is not valid"), *QuestType);
			}

			Q.Instigator = Quest->GetStringField(TEXT("Instigator"));
			Q.Ender = Quest->GetStringField(TEXT("Ender"));
			
			TArray<TSharedPtr<FJsonValue>> Rewards = Quest->GetArrayField(TEXT("Rewards"));
			LoadQuestRewards(Rewards, Q.Rewards);

			TArray<TSharedPtr<FJsonValue>> SubQuests = Quest->GetArrayField(TEXT("SubQuests"));
			LoadSubQuests(SubQuests, Q.SubQuests);

			// 진행도 별 Dialogue를 로드
			TSharedPtr<FJsonObject> Dialogues = Quest->GetObjectField(TEXT("Dialogues"));
			TArray<TSharedPtr<FJsonValue>> Dialogues_U = Dialogues->GetArrayField(TEXT("Unavailable"));
			TArray<TSharedPtr<FJsonValue>> Dialogues_A = Dialogues->GetArrayField(TEXT("Available"));
			TArray<TSharedPtr<FJsonValue>> Dialogues_P = Dialogues->GetArrayField(TEXT("InProgress"));
			TArray<TSharedPtr<FJsonValue>> Dialogues_F = Dialogues->GetArrayField(TEXT("Finished"));
			LoadQuestDialogues(Dialogues_U, Q.DialogueTree_Unavailable);
			LoadQuestDialogues(Dialogues_A, Q.DialogueTree_Available);
			LoadQuestDialogues(Dialogues_P, Q.DialogueTree_InProgress);
			LoadQuestDialogues(Dialogues_F, Q.DialogueTree_Finished);

			Q.Progress = EQuestProgress::Available;

			OutVal.Add(Q);
		}
	}
	// Json 오브젝트를 세이브 슬롯 리스트로 변환
	static bool LoadSlotList(TArray<FString>& OutVal)
	{
		// SaveSlotList.json 파일 로드
		FString JsonString;
		bool res = FFileHelper::LoadFileToString(JsonString, *(FPaths::ProjectSavedDir() + "Core/SaveSlotList.json"));
		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(Reader, JsonObject);

		TArray<TSharedPtr<FJsonValue>> SaveSlots = JsonObject->GetArrayField(TEXT("SaveSlots"));
		// String 배열로 전환
		for (int i = 0; i < SaveSlots.Num(); i++)
		{
			OutVal.Add(SaveSlots[i]->AsString());
		}

		return res;
	}
	// Json 오브젝트를 FSubQuestStatus로 변환
	static TArray<FSubQuestStatus> LoadSubQuestStatus(TArray<TSharedPtr<FJsonValue>> SubQuestStatus)
	{
		TArray<FSubQuestStatus> OutVal;

		for (int i = 0; i < SubQuestStatus.Num(); i++)
		{
			TSharedPtr<FJsonObject> SQS = SubQuestStatus[i]->AsObject();

			FSubQuestStatus OV;
			OV.Completed = SQS->GetBoolField(TEXT("Completed"));	// 퀘스트 완료 여부

			TArray<TSharedPtr<FJsonValue>> CA = SQS->GetArrayField(TEXT("CurrAmount"));
			for (int j = 0; j < CA.Num(); j++)
			{
				OV.CurrAmount.Add(CA[j]->AsNumber());	// 현재 처치/획득 수
			}
			
			OutVal.Add(OV);
		}

		return OutVal;
	}
	// Json 오브젝트를 FQuestStatus로 변환
	static TArray<FQuestStatus> LoadQuestStatus(TArray<TSharedPtr<FJsonValue>> QuestStatus)
	{
		TArray<FQuestStatus> OutVal;

		for (int i = 0; i < QuestStatus.Num(); i++)
		{
			TSharedPtr<FJsonObject> QS = QuestStatus[i]->AsObject();

			FQuestStatus OV;
			OV.QuestIndex = QS->GetIntegerField(TEXT("Index"));
			OV.Progress = EnumifyQuestProgressType(QS->GetStringField(TEXT("Progress")));
			OV.CurrPhase = QS->GetIntegerField(TEXT("CurrPhase"));
			OV.Remains = QS->GetIntegerField(TEXT("Remains"));
			OV.SubStatus = LoadSubQuestStatus(QS->GetArrayField(TEXT("SubStatus")));

			OutVal.Add(OV);
		}

		return OutVal;
	}
	// Json 오브젝트를 FShopItemShortForm으로 변환
	static TArray<FShopItemShortForm> LoadShopItems(TArray<TSharedPtr<FJsonValue>> ShopItems)
	{
		TArray<FShopItemShortForm> OutVal;

		for (int i = 0; i < ShopItems.Num(); i++)
		{
			TSharedPtr<FJsonObject> ShopItem = ShopItems[i]->AsObject();

			FShopItemShortForm OV;
			OV.ItemInfo = LoadItemShortForm(ShopItem->GetObjectField("ItemInfo"));
			OV.Price = ShopItem->GetIntegerField(TEXT("Price"));

			OutVal.Add(OV);
		}
		
		return OutVal;
	}
	// Json 오브젝트를 FActorMemory로 변환
	static FActorMemory LoadActorMemory(TSharedPtr<FJsonObject> ActorMemory)
	{
		FActorMemory OutVal;
		OutVal.Name = ActorMemory->GetStringField(TEXT("Name"));
		OutVal.IsValid = ActorMemory->GetBoolField(TEXT("Valid"));
		OutVal.CurrentPos = LoadVector(ActorMemory->GetArrayField(TEXT("CurrentPos")));
		OutVal.CurrentRot = LoadRotator(ActorMemory->GetObjectField(TEXT("CurrentRot")));

		return OutVal;
	}
	// Json 배열을 FActorMemory 배열로 변환
	static TArray<FActorMemory> LoadActorMemories(TArray<TSharedPtr<FJsonValue>> ActorMemories)
	{
		TArray<FActorMemory> OutVal;

		for (int i = 0; i < ActorMemories.Num(); i++)
		{
			TSharedPtr<FJsonObject> ActorMemory = ActorMemories[i]->AsObject();
			OutVal.Add(LoadActorMemory(ActorMemory));
		}

		return OutVal;
	}
	// Json 배열을 FNPCMemory 배열로 변환
	static TArray<FNPCMemory> LoadNPCMemories(TArray<TSharedPtr<FJsonValue>> NPCMemories)
	{
		TArray<FNPCMemory> OutVal;

		for (int i = 0; i < NPCMemories.Num(); i++)
		{
			TSharedPtr<FJsonObject> NPCMemory = NPCMemories[i]->AsObject();
			FNPCMemory OV;
			OV.DisplayName = NPCMemory->GetStringField(TEXT("Name"));
			OV.ActorMemory = LoadActorMemory(NPCMemory->GetObjectField(TEXT("ActorMemory")));
			OV.DialoguePhase = NPCMemory->GetIntegerField(TEXT("DialoguePhase"));
			OV.ShopItems = LoadShopItems(NPCMemory->GetArrayField(TEXT("ShopItems")));
			OV.AcquireItems = LoadItemShortForms(NPCMemory->GetArrayField(TEXT("AcquireItems")));

			OutVal.Add(OV);
		}

		return OutVal;
	}
	// Json 배열을 FMapMemory 배열로 변환
	static void LoadMapMemories(TArray<TSharedPtr<FJsonValue>> MapMemories, TArray<FMapMemory>& OutVal)
	{
		for (int i = 0; i < MapMemories.Num(); i++)
		{
			TSharedPtr<FJsonObject> MapMemory = MapMemories[i]->AsObject();
			
			FMapMemory OV;
			OV.Name = MapMemory->GetStringField(TEXT("Name"));
			OV.LocalNPCMemory = LoadNPCMemories(MapMemory->GetArrayField(TEXT("LocalNPCs")));
			OV.OriginActorMemory_CollectableItem = LoadActorMemories(MapMemory->GetArrayField(TEXT("LocalItems")));
			OutVal.Add(OV);
		}
	}
	// Json 오브젝트를 FWrappedItemForm 배열로 변환
	static TArray<FWrappedItemForm> LoadInventory(TSharedPtr<FJsonObject> Inventory)
	{
		TArray<FWrappedItemForm> OutVal;
		OutVal.SetNum(3);

		// 의상 인벤토리
		TArray<TSharedPtr<FJsonValue>> Clothes = Inventory->GetArrayField(TEXT("Clothes"));
		for (int i = 0; i < Clothes.Num(); i++)
		{
			TSharedPtr<FJsonObject> Cloth = Clothes[i]->AsObject();

			FItemForm Itemform;
			Itemform.ShortForm = LoadItemShortForm(Cloth);
			OutVal[(uint8)ETypeTag::Cloth].ItemForms.Add(Itemform);
		}
		// 무기 인벤토리
		TArray<TSharedPtr<FJsonValue>> Weapons = Inventory->GetArrayField(TEXT("Weapons"));
		for (int i = 0; i < Weapons.Num(); i++)
		{
			TSharedPtr<FJsonObject> Weapon = Weapons[i]->AsObject();

			FItemForm Itemform;
			Itemform.ShortForm = LoadItemShortForm(Weapon);
			OutVal[(uint8)ETypeTag::Weapon].ItemForms.Add(Itemform);
		}
		// 아이템 인벤토리
		TArray<TSharedPtr<FJsonValue>> Items = Inventory->GetArrayField(TEXT("Items"));
		for (int i = 0; i < Items.Num(); i++)
		{
			TSharedPtr<FJsonObject> Item = Items[i]->AsObject();

			FItemForm Itemform;
			Itemform.ShortForm = LoadItemShortForm(Item);
			OutVal[(uint8)ETypeTag::Item].ItemForms.Add(Itemform);
		}

		return OutVal;
	}
	// Json 오브젝트를 퀵슬롯 배열로 변환
	static TArray<int> LoadQuickslot(TSharedPtr<FJsonObject> Quickslot)
	{
		TArray<int> OutVal;
		OutVal.SetNum(3);
		OutVal[(uint8)ETypeTag::Cloth] = Quickslot->GetIntegerField(TEXT("Cloth"));
		OutVal[(uint8)ETypeTag::Weapon] = Quickslot->GetIntegerField(TEXT("Weapon"));
		OutVal[(uint8)ETypeTag::Item] = Quickslot->GetIntegerField(TEXT("Item"));

		return OutVal;
	}
	// Json 오브젝트를 FCharacterMemory로 변환
	static void LoadCharacterMemory(TSharedPtr<FJsonObject> CharacterMemory, FCharacterMemory& OutVal)
	{
		OutVal.DisplayName = CharacterMemory->GetStringField(TEXT("DisplayName")); // 이름
		OutVal.CurrentLevel = CharacterMemory->GetIntegerField(TEXT("CurrentLevel")); // 레벨
		OutVal.CurrentMap = CharacterMemory->GetStringField(TEXT("CurrentMap")); // 위치한 맵
		OutVal.CurrentPos = LoadVector(CharacterMemory->GetArrayField(TEXT("CurrentPos"))); // 위치
		OutVal.CurrentRot = LoadRotator(CharacterMemory->GetObjectField(TEXT("CurrentRot"))); // 회전 상태
		OutVal.CurrentHP = CharacterMemory->GetIntegerField(TEXT("CurrentHP")); // 체력
		OutVal.CurrentMoney = CharacterMemory->GetIntegerField(TEXT("CurrentMoney")); // 돈
		OutVal.Inventory = LoadInventory(CharacterMemory->GetObjectField(TEXT("Inventory"))); // 인벤토리
		OutVal.Quickslots_Before = LoadQuickslot(CharacterMemory->GetObjectField(TEXT("Quickslot_Before"))); // 퀵슬롯-이전
		OutVal.Quickslots_Now = LoadQuickslot(CharacterMemory->GetObjectField(TEXT("Quickslot_Now"))); // 퀵슬록-현재
		OutVal.Quickslots_Next = LoadQuickslot(CharacterMemory->GetObjectField(TEXT("Quickslot_Next"))); // 퀵슬록-다음
	}
};

// 구조체를 Json 오브젝트/파일로 변환
class JsonSaver
{
public:
	// Json 오브젝트를 해당 경로의 파일에 저장
	static bool SaveObjectToJson(TSharedPtr<FJsonObject> Obj, FString FilePath)
	{
		FString FileStr;
		TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<>::Create(&FileStr);

		FJsonSerializer::Serialize(Obj.ToSharedRef(), Writer);
		return FFileHelper::SaveStringToFile(*FileStr, *FilePath);
	}
	// ETypeTag를 Json 오브젝트로 변환
	static FString StringifyItemType(ETypeTag Type)
	{
		if (Type == ETypeTag::Cloth) return "Cloth";
		if (Type == ETypeTag::Weapon) return "Weapon";
		if (Type == ETypeTag::Item) return "Item";

		return "None";
	}
	// EQuestProgress를 Json 오브젝트로 변환
	static FString StringifyQuestProgress(EQuestProgress Type)
	{
		if (Type == EQuestProgress::Unavailable) return "Unavailable";
		if (Type == EQuestProgress::Available) return "Available";
		if (Type == EQuestProgress::InProgress) return "InProgress";
		if (Type == EQuestProgress::Finished) return "Finished";
		if (Type == EQuestProgress::AlreadyDone) return "AlreadyDone";

		return "None";
	}
	// 벡터를 Json 배열로 변환
	static TArray<TSharedPtr<FJsonValue>> SaveVector(FVector Vector)
	{
		TArray<TSharedPtr<FJsonValue>> JsonVector;

		TSharedPtr<FJsonObject> VectorObj = MakeShareable(new FJsonObject);
		for (int i = 0; i < 3; i++)
		{
			JsonVector.Add(MakeShareable(new FJsonValueNumber(Vector[i])));
		}

		return JsonVector;
	}
	// FRotator를 Json 오브젝트로 변환
	static TSharedPtr<FJsonObject> SaveRotator(FRotator Rotator)
	{
		TSharedPtr<FJsonObject> JsonRotator = MakeShareable(new FJsonObject());
		JsonRotator->SetNumberField(TEXT("Roll"), Rotator.Roll);
		JsonRotator->SetNumberField(TEXT("Pitch"), Rotator.Pitch);
		JsonRotator->SetNumberField(TEXT("Yaw"), Rotator.Yaw);

		return JsonRotator;
	}
	// FItemShortForm을 Json 오브젝트로 변환
	static TSharedPtr<FJsonObject> SaveItemShortForm(FItemShortForm ItemShortForm)
	{
		TSharedPtr<FJsonObject> ItemShortFormObj = MakeShareable(new FJsonObject());

		ItemShortFormObj->SetStringField(TEXT("Type"), StringifyItemType(ItemShortForm.TypeTag));
		ItemShortFormObj->SetStringField(TEXT("Name"), ItemShortForm.NameTag);
		ItemShortFormObj->SetNumberField(TEXT("Code"), ItemShortForm.Code);
		ItemShortFormObj->SetNumberField(TEXT("Num"), ItemShortForm.Num);
		ItemShortFormObj->SetBoolField(TEXT("Sellable"), ItemShortForm.bIsSellable);

		return ItemShortFormObj;
	}
	// FItemShortForm 배열을 Json 배열로 변환
	static TArray<TSharedPtr<FJsonValue>> SaveItemShortForms(TArray<FItemShortForm> ItemShortForms)
	{
		TArray<TSharedPtr<FJsonValue>> OutVals;

		for (int i = 0; i < ItemShortForms.Num(); i++)
		{
			OutVals.Add(MakeShareable(new FJsonValueObject(SaveItemShortForm(ItemShortForms[i]))));
		}

		return OutVals;
	}
	// FWrappedItemForm 배열을 Json 오브젝트로 변환 (플레이어의 인벤토리)
	static TSharedPtr<FJsonObject> SaveInventory(TArray<FWrappedItemForm> Inventory)
	{
		TSharedPtr<FJsonObject> JsonInventory = MakeShareable(new FJsonObject());
		
		// 의상 인벤토리
		TArray<TSharedPtr<FJsonValue>> JsonClothes;
		for (int i = 0; i < Inventory[(uint8)ETypeTag::Cloth].ItemForms.Num(); i++)
		{
			FItemShortForm Cloth = Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].ShortForm;

			JsonClothes.Add(MakeShareable(new FJsonValueObject(SaveItemShortForm(Cloth))));
		}
		JsonInventory->SetArrayField(TEXT("Clothes"), JsonClothes);
		// 무기 인벤토리
		TArray<TSharedPtr<FJsonValue>> JsonWeapons;
		for (int i = 0; i < Inventory[(uint8)ETypeTag::Weapon].ItemForms.Num(); i++)
		{
			FItemShortForm Weapon = Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].ShortForm;

			JsonWeapons.Add(MakeShareable(new FJsonValueObject(SaveItemShortForm(Weapon))));
		}
		JsonInventory->SetArrayField(TEXT("Weapons"), JsonWeapons);
		// 아이템 인벤토리
		TArray<TSharedPtr<FJsonValue>> JsonItems;
		for (int i = 0; i < Inventory[(uint8)ETypeTag::Item].ItemForms.Num(); i++)
		{
			FItemShortForm Item = Inventory[(uint8)ETypeTag::Item].ItemForms[i].ShortForm;

			JsonItems.Add(MakeShareable(new FJsonValueObject(SaveItemShortForm(Item))));
		}
		JsonInventory->SetArrayField(TEXT("Items"), JsonItems);
		
		return JsonInventory;
	}
	// FWrappedItemForm 배열을 Json 오브젝트로 변환 (NPC의 상점 인벤토리)
	static TArray<TSharedPtr<FJsonValue>> SaveShopItems(TArray<FShopItemShortForm> ShopItems)
	{
		TArray<TSharedPtr<FJsonValue>> ShopItemsMem;
		
		for (int i = 0; i < ShopItems.Num(); i++)
		{
			TSharedPtr<FJsonObject> ShopItemObj = MakeShareable(new FJsonObject());

			ShopItemObj->SetObjectField(TEXT("ItemInfo"), SaveItemShortForm(ShopItems[i].ItemInfo));
			ShopItemObj->SetNumberField(TEXT("Price"), ShopItems[i].Price);

			ShopItemsMem.Add(MakeShareable(new FJsonValueObject(ShopItemObj)));
		}

		return ShopItemsMem;
	}
	// 퀵슬롯 배열을 Json 오브젝트로 변환
	static TSharedPtr<FJsonObject> SaveQuickSlot(TArray<int> Quickslot)
	{
		TSharedPtr<FJsonObject> QuickslotObj = MakeShareable(new FJsonObject());

		QuickslotObj->SetNumberField(TEXT("Cloth"), Quickslot[(uint8)ETypeTag::Cloth]);
		QuickslotObj->SetNumberField(TEXT("Weapon"), Quickslot[(uint8)ETypeTag::Weapon]);
		QuickslotObj->SetNumberField(TEXT("Item"), Quickslot[(uint8)ETypeTag::Item]);

		return QuickslotObj;
	}
	// FCharacterMemory를 Json 오브젝트로 변환
	static TSharedPtr<FJsonObject> SaveCharacterMemory(FCharacterMemory CharacterMemory)
	{
		TSharedPtr<FJsonObject> ChMemObj = MakeShareable(new FJsonObject());

		ChMemObj->SetStringField(TEXT("DisplayName"), CharacterMemory.DisplayName); // 이름
		ChMemObj->SetNumberField(TEXT("CurrentLevel"), CharacterMemory.CurrentLevel); // 레벨
		ChMemObj->SetStringField(TEXT("CurrentMap"), CharacterMemory.CurrentMap); // 위치한 맵
		ChMemObj->SetArrayField(TEXT("CurrentPos"), SaveVector(CharacterMemory.CurrentPos)); // 위치
		ChMemObj->SetObjectField(TEXT("CurrentRot"), SaveRotator(CharacterMemory.CurrentRot)); // 회전 상태
		ChMemObj->SetNumberField(TEXT("CurrentHP"), CharacterMemory.CurrentHP); // 체력
		ChMemObj->SetNumberField(TEXT("CurrentMoney"), CharacterMemory.CurrentMoney); // 돈
		ChMemObj->SetObjectField(TEXT("Inventory"), SaveInventory(CharacterMemory.Inventory)); // 인벤토리
		ChMemObj->SetObjectField(TEXT("Quickslot_Before"), SaveQuickSlot(CharacterMemory.Quickslots_Before)); // 퀵슬롯-이전
		ChMemObj->SetObjectField(TEXT("Quickslot_Now"), SaveQuickSlot(CharacterMemory.Quickslots_Now)); // 퀵슬록-현재
		ChMemObj->SetObjectField(TEXT("Quickslot_Next"), SaveQuickSlot(CharacterMemory.Quickslots_Next)); // 퀵슬록-다음
		
		return ChMemObj;
	}
	// FActorMemory를 Json 오브젝트로 변환
	static TSharedPtr<FJsonObject> SaveActorMemory(FActorMemory ActorMemory)
	{
		TSharedPtr<FJsonObject> ActorMemObj = MakeShareable(new FJsonObject());

		ActorMemObj->SetStringField(TEXT("Name"), ActorMemory.Name);
		ActorMemObj->SetBoolField(TEXT("Valid"), ActorMemory.IsValid);
		ActorMemObj->SetArrayField(TEXT("CurrentPos"), SaveVector(ActorMemory.CurrentPos));
		ActorMemObj->SetObjectField(TEXT("CurrentRot"), SaveRotator(ActorMemory.CurrentRot));

		return ActorMemObj;
	}
	// FNPCMemory 배열을 Json 배열로 변환
	static TArray<TSharedPtr<FJsonValue>> SaveNPCMemories(TArray<FNPCMemory> NPCMemory)
	{
		TArray<TSharedPtr<FJsonValue>> NPCMemories;

		for (int i = 0; i < NPCMemory.Num(); i++)
		{
			TSharedPtr<FJsonObject> NPCMemObj = MakeShareable(new FJsonObject());

			NPCMemObj->SetStringField(TEXT("Name"), NPCMemory[i].DisplayName);
			NPCMemObj->SetObjectField(TEXT("ActorMemory"), SaveActorMemory(NPCMemory[i].ActorMemory));
			NPCMemObj->SetNumberField(TEXT("DialoguePhase"), NPCMemory[i].DialoguePhase);
			NPCMemObj->SetArrayField(TEXT("ShopItems"), SaveShopItems(NPCMemory[i].ShopItems));
			NPCMemObj->SetArrayField(TEXT("AcquireItems"), SaveItemShortForms(NPCMemory[i].AcquireItems));

			NPCMemories.Add(MakeShareable(new FJsonValueObject(NPCMemObj)));
		}

		return NPCMemories;
	}
	// FActorMemory 배열을 Json 배열로 변환
	static TArray<TSharedPtr<FJsonValue>> SaveActorMemories(TArray<FActorMemory> ActorMemory)
	{
		TArray<TSharedPtr<FJsonValue>> ActorMemories;

		for (int i = 0; i < ActorMemory.Num(); i++)
		{
			ActorMemories.Add(MakeShareable(new FJsonValueObject(SaveActorMemory(ActorMemory[i]))));
		}

		return ActorMemories;
	}
	// FMapMemory를 Json 오브젝트로 변환
	static TSharedPtr<FJsonObject> SaveMapMemory(FMapMemory MapMemory)
	{
		TSharedPtr<FJsonObject> MapMemObj = MakeShareable(new FJsonObject());

		MapMemObj->SetStringField(TEXT("Name"), MapMemory.Name);
		MapMemObj->SetArrayField(TEXT("LocalNPCs"), SaveNPCMemories(MapMemory.LocalNPCMemory));
		MapMemObj->SetArrayField(TEXT("LocalItems"), SaveActorMemories(MapMemory.OriginActorMemory_CollectableItem));

		return MapMemObj;
	}
	// FSubQuestStatus 배열을 Json 배열로 변환
	static TArray<TSharedPtr<FJsonValue>> SaveSubQuestStatuses(TArray<FSubQuestStatus> SubQuestStatus)
	{
		TArray<TSharedPtr<FJsonValue>> SQSS;
		for (int i = 0; i < SubQuestStatus.Num(); i++)
		{
			TSharedPtr<FJsonObject> SQS = MakeShareable(new FJsonObject());

			SQS->SetBoolField(TEXT("Completed"), SubQuestStatus[i].Completed);
			TArray<TSharedPtr<FJsonValue>> Amount;
			for (int j = 0; j < SubQuestStatus[i].CurrAmount.Num(); j++)	// 처치/획득 수
			{
				Amount.Add(MakeShareable(new FJsonValueNumber(SubQuestStatus[i].CurrAmount[j])));
			}
			SQS->SetArrayField(TEXT("CurrAmount"), Amount);

			SQSS.Add(MakeShareable(new FJsonValueObject(SQS)));
		}

		return SQSS;
	}
	// FQuestStatus 배열을 Json 배열로 변환
	static TArray<TSharedPtr<FJsonValue>> SaveQuestStatuses(TArray<FQuestStatus> QuestStatus)
	{
		TArray<TSharedPtr<FJsonValue>> QSS;
		for (int i = 0; i < QuestStatus.Num(); i++)
		{
			TSharedPtr<FJsonObject> QS = MakeShareable(new FJsonObject());

			QS->SetNumberField(TEXT("Index"), QuestStatus[i].QuestIndex);
			QS->SetStringField(TEXT("Progress"), StringifyQuestProgress(QuestStatus[i].Progress));
			QS->SetNumberField(TEXT("CurrPhase"), QuestStatus[i].CurrPhase);
			QS->SetNumberField(TEXT("Remains"), QuestStatus[i].Remains);
			QS->SetArrayField(TEXT("SubStatus"), SaveSubQuestStatuses(QuestStatus[i].SubStatus));

			QSS.Add(MakeShareable(new FJsonValueObject(QS)));
		}

		return QSS;
	}
	// 스트링 배열을 SaveSlotList.json에 저장
	static bool SaveSlotList(TArray<FString> SlotList)
	{
		FString FilePath = *(FPaths::ProjectSavedDir() + "Core/SaveSlotList.json");
		TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());

		TArray<TSharedPtr<FJsonValue>> Slots;
		for (int i = 0; i < SlotList.Num(); i++)
		{
			Slots.Add(MakeShareable(new FJsonValueString(SlotList[i])));
		}

		Obj->SetArrayField(TEXT("SaveSlots"), Slots);

		return SaveObjectToJson(Obj, FilePath);
	}
};