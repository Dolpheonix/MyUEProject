// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/CapsuleComponent.h"
#include "../../quickstartGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../../UI/DialogueBox.h"
#include "../../UI/Shop.h"
#include "../../Utils/Helpers.h"
#include "../../quickstart.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	auto Capsule = Cast<UCapsuleComponent>(RootComponent);
	Capsule->SetCollisionProfileName("Enemy");

	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetCollisionProfileName("NoCollision");
	MainMesh->SetGenerateOverlapEvents(true);

	SelfCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SelfCam"));
	Helpers::SetComponent(&SelfCamera, RootComponent, FVector(0, 0, 0), FRotator(0, 0, 0));

	ClothTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Cloth_Sheet.Cloth_Sheet"));
	WeaponTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Weapon_Sheet.Weapon_Sheet"));
	ItemTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Item_Sheet.Item_Sheet"));
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	for (int i = 0; i < ShopItemsInfo.Num(); i++)
	{
		FItemShortForm iteminfo = ShopItemsInfo[i];

		if (iteminfo.InfoTag == "Invalid Item, Check data table")
		{
			UE_LOG(ErrAsset, Error, TEXT("%s : No such item exist"), *iteminfo.NameTag);
		}

		FItemForm registerform(iteminfo);

		registerform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
		ShopItems.Add(registerform);
	}
}

void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bInteracted && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 200.0f)
	{
		bInteractable = true;
		if (Player->GetCurrentAction() == ECustomActionMode::INTERACT)
		{
			Interact();
		}
	}
	else
	{
		bInteractable = false;
	}
}

void ANPC::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& e)
{
	Super::PostEditChangeChainProperty(e);

	FString prop = e.MemberProperty->GetName();

	if (prop == "ShopItemsInfo")
	{
		int32 index = e.GetArrayIndex(TEXT("ShopItemsInfo"));
		
		if (e.ChangeType == EPropertyChangeType::ArrayAdd)
		{
			UE_LOG(LogTemp, Log, TEXT("Added"));
			if (ShopItemsInfo.Num() - 1 == index) // Added in tail
			{
				Prices.Add(0);
			}
			else // Added in Midline
			{
				Prices.EmplaceAt(index);
			}
		}
		else if (e.ChangeType == EPropertyChangeType::ArrayRemove)
		{
			Prices.RemoveAt(index);
		}
		else if (e.ChangeType == EPropertyChangeType::ArrayClear)
		{
			Prices.Empty();
		}

		UE_LOG(LogTemp, Log, TEXT("%d"), index);
	}
	else if (prop == "NameTag" || prop == "TypeTag")
	{
		FString name = e.Property->GetName();
		int32 index = e.GetArrayIndex(TEXT("ShopItemsInfo"));
		int code = -1;
		if (index >= 0)
		{
			switch (ShopItemsInfo[index].TypeTag)
			{
			case ETypeTag::Cloth:
				ShopItemsInfo[index].InfoTag = Helpers::FindInfo(ClothTable, ETypeTag::Cloth, ShopItemsInfo[index].NameTag, code);
				break;
			case ETypeTag::Item:
				ShopItemsInfo[index].InfoTag = Helpers::FindInfo(ItemTable, ETypeTag::Item, ShopItemsInfo[index].NameTag, code);
				break;
			case ETypeTag::Weapon:
				ShopItemsInfo[index].InfoTag = Helpers::FindInfo(WeaponTable, ETypeTag::Weapon, ShopItemsInfo[index].NameTag, code);
				ShopItemsInfo[index].Code = code;
				break;
			default:
				break;
			}
		}
		else
		{
			index = e.GetArrayIndex(TEXT("AcquireItemsInfo"));
			switch (AcquireItemsInfo[index].TypeTag)
			{
			case ETypeTag::Cloth:
				AcquireItemsInfo[index].InfoTag = Helpers::FindInfo(ClothTable, ETypeTag::Cloth, AcquireItemsInfo[index].NameTag, code);
				break;
			case ETypeTag::Item:
				AcquireItemsInfo[index].InfoTag = Helpers::FindInfo(ItemTable, ETypeTag::Item, AcquireItemsInfo[index].NameTag, code);
				break;
			case ETypeTag::Weapon:
				AcquireItemsInfo[index].InfoTag = Helpers::FindInfo(WeaponTable, ETypeTag::Weapon, AcquireItemsInfo[index].NameTag, code);
				AcquireItemsInfo[index].Code = code;
				break;
			default:
				break;
			}
		}
	}
}

void ANPC::OnHurt()
{

}

void ANPC::OnDead()
{

}

void ANPC::Interact()
{
	bInteracted = true;

	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.0f);

	AquickstartGameModeBase* gamemode = Cast<AquickstartGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeMenuWidget(gamemode->DialogueBoxUI);

	Cast<UDialogueBox>(gamemode->DialogueBoxUI)->InitDialogue(DialogueTree, this);
}

void ANPC::UnInteract()
{
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Player, 1.0f);
	bInteracted = false;
	Player->GameMode->ChangeMenuWidget(Player->GameMode->MainUI);
}

void ANPC::OpenShop()
{
	AquickstartGameModeBase* gamemode = Cast<AquickstartGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeMenuWidget(gamemode->ShopUI);

	Cast<UShop>(gamemode->ShopUI)->InitShop(this);
}

void ANPC::OpenQuestDialogue(int index)
{
	if (index >= Quests.Num())
	{
		UE_LOG(ErrNPC, Log, TEXT("Index >= Array.Num()"));
	}
	else
	{
		AquickstartGameModeBase* gamemode = Cast<AquickstartGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		Cast<UDialogueBox>(gamemode->DialogueBoxUI)->InitQuestDialogue(index);
	}
}

void ANPC::GiveQuest(int index)
{
	Quests[index].Progress = EQuestProgress::InProgress;
	Player->RegisterQuest(Quests[index]);
}

void ANPC::EndQuest(int index)
{
	Quests[index].Progress = EQuestProgress::AlreadyDone;
	Player->EndQuest(Quests[index]);
}