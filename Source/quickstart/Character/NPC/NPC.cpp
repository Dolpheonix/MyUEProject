// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/CapsuleComponent.h"
#include "../../Core/GameMode/MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../../UI/DialogueBox.h"
#include "../../UI/Shop.h"
#include "../../Utils/Helpers.h"
#include "Blueprint/WidgetTree.h"
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
		FItemShortForm iteminfo = ShopItemsInfo[i].ItemInfo;

		if (iteminfo.InfoTag == "Invalid Item, Check data table")
		{
			UE_LOG(ErrAsset, Error, TEXT("%s : No such item exist"), *iteminfo.NameTag);
		}

		FItemForm itemform(iteminfo);

		itemform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		itemform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		itemform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));

		FShopItemForm registerform;
		registerform.ItemForm = itemform;
		registerform.Price = ShopItemsInfo[i].Price;

		ShopItems.Add(registerform);
	}

	for (FQuest quest : Quests)
	{
		for (FSingleQuest subquest : quest.SubQuests)
		{
			if (subquest.Type == ESingleQuestType::Action)
			{
				AMainGameMode* gamemode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

				if (!gamemode->ActionCodeTable[subquest.ActionCode])
				{
					gamemode->ActionCodeTable[subquest.ActionCode] = true;
				}
				else
				{
					UE_LOG(ErrQuest, Error, TEXT("Action Code Conflict at %i"), subquest.ActionCode);
				}
			}
		}
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

	if (prop == "NameTag" || prop == "TypeTag")
	{
		FString name = e.Property->GetName();
		int32 index = e.GetArrayIndex(TEXT("ShopItemsInfo"));
		int code = -1;
		if (index >= 0)
		{
			switch (ShopItemsInfo[index].ItemInfo.TypeTag)
			{
			case ETypeTag::Cloth:
				ShopItemsInfo[index].ItemInfo.InfoTag = Helpers::FindInfo(ClothTable, ETypeTag::Cloth, ShopItemsInfo[index].ItemInfo.NameTag, code);
				break;
			case ETypeTag::Item:
				ShopItemsInfo[index].ItemInfo.InfoTag = Helpers::FindInfo(ItemTable, ETypeTag::Item, ShopItemsInfo[index].ItemInfo.NameTag, code);
				break;
			case ETypeTag::Weapon:
				ShopItemsInfo[index].ItemInfo.InfoTag = Helpers::FindInfo(WeaponTable, ETypeTag::Weapon, ShopItemsInfo[index].ItemInfo.NameTag, code);
				ShopItemsInfo[index].ItemInfo.Code = code;
				break;
			default:
				break;
			}
		}
		else
		{
			index = e.GetArrayIndex(TEXT("AcquireItemsInfo"));
			if (index >= 0)
			{
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

	AMainGameMode* gamemode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeMenuWidget(gamemode->DialogueBoxUI);

	Cast<UDialogueBox>(gamemode->DialogueBoxUI)->OpenUI(this);
}

void ANPC::UnInteract()
{
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Player, 1.0f);
	bInteracted = false;

	Player->GameMode->ChangeMenuWidget(Player->GameMode->MainUI);
	Player->Notify();
}

void ANPC::OpenShop()
{
	AMainGameMode* gamemode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeMenuWidget(gamemode->ShopUI);

	Cast<UShop>(gamemode->ShopUI)->InitShop(this);
}