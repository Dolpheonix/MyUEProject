// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "../../quickstart.h"
#include "../SaveObject/GameSaver.h"
#include "../GameMode/MainGameMode.h"
#include "Kismet/GameplayStatics.h"

UMainGameInstance::UMainGameInstance()
{
}

void UMainGameInstance::Init()
{
	USaveSlots* SaveSlots = Cast<USaveSlots>(UGameplayStatics::LoadGameFromSlot("SaveSlots", 10));
	// Create New SaveSlots List
	if (!SaveSlots)
	{
		SaveSlots = Cast<USaveSlots>(UGameplayStatics::CreateSaveGameObject(USaveSlots::StaticClass()));
		SaveSlots->SlotNames.SetNum(MAX_SAVE_SLOTS);
		for (int i = 0; i < MAX_SAVE_SLOTS; i++)
		{
			SaveSlots->SlotNames[i].Empty();
		}
		SaveSlots->SlotNames[0] = CharacterMemory.Name;

		UGameplayStatics::SaveGameToSlot(SaveSlots, "SaveSlots", 10);
	}
}

void UMainGameInstance::Shutdown()
{
	SaveToFile();
	Super::Shutdown();
}

void UMainGameInstance::InitializeMemory(FString Name)
{
	CharacterMemory.Name = Name;
	CharacterMemory.CurrentPos = FVector(0.0f, 0.0f, -100.0f); // Impossible region ==> start at Level's PlayerStart region
	CharacterMemory.HP = 100.0f;
	CharacterMemory.Money = 10000;

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

	fist.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Fist_Normal.Fist_Normal"));
	fist.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/Fist_Hovered.Fist_Hovered"));
	fist.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/Fist_Selected.Fist_Selected"));
	noitem.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoItem_Normal.NoItem_Normal"));
	noitem.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoItem_Hovered.NoItem_Hovered"));
	noitem.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoItem_Selected.NoItem_Selected"));
	nocloth.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoItem_Normal.NoItem_Normal"));
	nocloth.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoItem_Hovered.NoItem_Hovered"));
	nocloth.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoItem_Selected.NoItem_Selected"));

	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Weapons
	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Items
	CharacterMemory.Inventory.Add(FWrappedItemForm()); // Clothes
	CharacterMemory.Inventory[uint8(ETypeTag::Weapon)].ItemForms.Add(fist);
	CharacterMemory.Inventory[uint8(ETypeTag::Item)].ItemForms.Add(noitem);
	CharacterMemory.Inventory[uint8(ETypeTag::Cloth)].ItemForms.Add(nocloth);

	CharacterMemory.Quickslots_Before = TArray<int>({ 0,0,0 });
	CharacterMemory.Quickslots_Now = TArray<int>({ 0,0,0 });
	CharacterMemory.Quickslots_Next = TArray<int>({ 0,0,0 });
}

void UMainGameInstance::LoadToCharacter(AMainCharacter* character)
{
	character->CurrLevel = CharacterMemory.CurrentLevel;
	character->Name = CharacterMemory.Name;
	if (CharacterMemory.CurrentPos.Z > 0)
	{
		character->SetActorLocation(CharacterMemory.CurrentPos);
		character->SetActorRotation(CharacterMemory.CurrentRot);
	}
	character->HP = CharacterMemory.HP;
	character->CurrMoney = CharacterMemory.Money;
	character->Inventory = CharacterMemory.Inventory;
	character->Quickslots_Before = CharacterMemory.Quickslots_Before;
	character->Quickslots_Now = CharacterMemory.Quickslots_Now;
	character->Quickslots_Next = CharacterMemory.Quickslots_Next;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Loaded!");
}

void UMainGameInstance::SaveFromCharacter(AMainCharacter* character)
{
	CharacterMemory.CurrentLevel = character->CurrLevel;
	CharacterMemory.CurrentMap = UGameplayStatics::GetCurrentLevelName(GetWorld());
	CharacterMemory.CurrentPos = character->GetActorLocation();
	CharacterMemory.CurrentRot = character->GetActorRotation();
	CharacterMemory.HP = character->HP;
	CharacterMemory.Money = character->CurrMoney;
	CharacterMemory.Inventory = character->Inventory;
	CharacterMemory.Quickslots_Before = character->Quickslots_Before;
	CharacterMemory.Quickslots_Now = character->Quickslots_Now;
	CharacterMemory.Quickslots_Next = character->Quickslots_Next;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Saved!");
}

void UMainGameInstance::LoadFromFile(FString slotname, int32 slotindex)
{
	UGameSaver* LoadFile = Cast<UGameSaver>(UGameplayStatics::LoadGameFromSlot(slotname, slotindex));
	if (LoadFile)
	{
		CharacterMemory = LoadFile->CharacterMemory;
		SlotIndex = slotindex;
	}
}

void UMainGameInstance::SaveToFile()
{
	UGameSaver* GameSaver = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	if (GameSaver)
	{
		GameSaver->SlotIndex = SlotIndex;
		GameSaver->CharacterMemory = CharacterMemory;
	}
	else
	{
		UE_LOG(ErrLoadSave, Error, TEXT("Can't generate save object"));
	}

	UGameplayStatics::SaveGameToSlot(GameSaver, CharacterMemory.Name, SlotIndex);
}

//void AMainGameInstance::Start()
//{
//	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Tutorial"));
//}

int32 UMainGameInstance::AllocateSlotIndex()
{
	USaveSlots* SaveSlots = Cast<USaveSlots>(UGameplayStatics::LoadGameFromSlot("SaveSlots", 10));
	if (!SaveSlots)
	{
		// Create New SaveSlots List
		SaveSlots = Cast<USaveSlots>(UGameplayStatics::CreateSaveGameObject(USaveSlots::StaticClass()));
		SaveSlots->SlotNames.SetNum(MAX_SAVE_SLOTS);
		for (int i = 0; i < MAX_SAVE_SLOTS; i++)
		{
			SaveSlots->SlotNames[i] = "";
		}
		SaveSlots->SlotNames[0] = CharacterMemory.Name;

		UGameplayStatics::SaveGameToSlot(SaveSlots, "SaveSlots", 10);
		SlotIndex = 0;
		return SlotIndex;
	}
	else
	{
		for (int i = 0; i < MAX_SAVE_SLOTS; i++)
		{
			if (SaveSlots->SlotNames[i] == "")
			{
				SaveSlots->SlotNames[i] = CharacterMemory.Name;
				SlotIndex = i;

				UGameplayStatics::SaveGameToSlot(SaveSlots, "SaveSlots", 10);
				return SlotIndex;
			}
			else if (SaveSlots->SlotNames[i] == CharacterMemory.Name)
			{
				return -1; // 같은 이름의 슬롯이 이미 존재
			}
		}

		return -2; // 세이브 슬롯이 꽉참
	}
}