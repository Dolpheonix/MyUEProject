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
	// Capsule Collision Component
	auto Capsule = Cast<UCapsuleComponent>(RootComponent);
	Capsule->SetCollisionProfileName("Enemy");
	// Main Mesh
	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetCollisionProfileName("NoCollision");
	MainMesh->SetGenerateOverlapEvents(true);
	// ī�޶�
	SelfCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SelfCam"));
	Helpers::SetComponent(&SelfCamera, RootComponent, FVector(0, 0, 0), FRotator(0, 0, 0));
	// ������ ���̺� �ε�
	ClothTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Cloth_Sheet.Cloth_Sheet"));
	WeaponTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Weapon_Sheet.Weapon_Sheet"));
	ItemTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Item_Sheet.Item_Sheet"));
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));	// ���� �÷��̾�

	for (int i = 0; i < ShopItemsInfo.Num(); i++)
	{
		FItemShortForm iteminfo = ShopItemsInfo[i].ItemInfo;

		// �������� Invalid�� ��� ���� �޽��� �˸�
		if (iteminfo.InfoTag == "Invalid Item, Check data table")
		{
			UE_LOG(ErrAsset, Error, TEXT("%s : No such item exist"), *iteminfo.NameTag);
		}

		FItemForm itemform(iteminfo);
		// ����� �ε�
		itemform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		itemform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		itemform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
		
		FShopItemForm registerform;
		registerform.ItemForm = itemform;
		registerform.Price = ShopItemsInfo[i].Price;
		// ���� �����ۿ� �߰�
		ShopItems.Add(registerform);
	}

	for (int i = 0; i < AcquireItemsInfo.Num(); i++)
	{
		FItemShortForm iteminfo = AcquireItemsInfo[i];

		if (iteminfo.InfoTag == "Invalid Item, Check data table")
		{
			UE_LOG(ErrAsset, Error, TEXT("%s : No such item exist"), *iteminfo.NameTag);
		}
	}
}

void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bInteracted && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 200.0f)	// ��ȣ�ۿ� ������ �ʰ�, �÷��̾ �ݰ濡 ����
	{
		bInteractable = true;
		if (Player->GetCurrentAction() == ECustomActionMode::INTERACT)	// �÷��̾ ��ȣ�ۿ� Ű�� ����
		{
			Interact();	// ��ȣ�ۿ�
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

	if (prop == "NameTag" || prop == "TypeTag")	// �������� �̸��̳�, Ÿ���� �ٲ���� ���
	{
		FString name = e.Property->GetName();
		int32 index = e.GetArrayIndex(TEXT("ShopItemsInfo"));	// ShopItemsInfo�� ������Ƽ�� �ٲ���� ���, �ε��� ��ȯ
		int code = -1;
		if (index >= 0)
		{
			// ���� ������Ƽ�� �´� �������� ������ �ε� �� ������Ƽ â�� ���
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
			index = e.GetArrayIndex(TEXT("AcquireItemsInfo"));	// AcquireItemsInfo�� ������Ƽ�� �ٲ�
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
	Player->bInteracting = true;	// �÷��̾ ��ȣ�ۿ� ���·� ����

	bInteracted = true;	// NPC�� ��ȣ�ۿ� ���·� ����

	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.0f);	// NPC�� ����ķ���� ���� Ÿ�� ����

	AMainGameMode* gamemode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeCurrentWidget(gamemode->DialogueBoxUI);	// ���̾�α� UI ��

	Cast<UDialogueBox>(gamemode->DialogueBoxUI)->OpenUI(this);	// ���̾�α� �ε�
}

void ANPC::UnInteract()
{
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Player, 1.0f);	// �÷��̾��� ī�޶�� ���ư�
	bInteracted = false;

	Player->GameMode->ChangeCurrentWidget(Player->GameMode->MainUI);	// ���� UI�� ����
	Player->Notify();	// ��ȣ�ۿ� �� ���� �˸� ǥ��
	Player->bInteracting = false;
}

void ANPC::OpenShop()
{
	AMainGameMode* gamemode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeCurrentWidget(gamemode->ShopUI);	// ���� UI ��

	Cast<UShop>(gamemode->ShopUI)->InitShop(this);	// ���� ������ �ε�
}

void ANPC::GenerateShopItems()
{
	ShopItems.Empty();	// BeginPlay()�� ���� �ڵ� ������ ������ ��� ���

	// ���̺� ���Ͽ� ���� ShopItemsInfo�� �ٲ�. �̸� ����
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
}