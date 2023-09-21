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
	// 카메라
	SelfCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SelfCam"));
	Helpers::SetComponent(&SelfCamera, RootComponent, FVector(0, 0, 0), FRotator(0, 0, 0));
	// 데이터 테이블 로드
	ClothTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Cloth_Sheet.Cloth_Sheet"));
	WeaponTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Weapon_Sheet.Weapon_Sheet"));
	ItemTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Item_Sheet.Item_Sheet"));
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));	// 메인 플레이어

	for (int i = 0; i < ShopItemsInfo.Num(); i++)
	{
		FItemShortForm iteminfo = ShopItemsInfo[i].ItemInfo;

		// 아이템이 Invalid한 경우 오류 메시지 알림
		if (iteminfo.InfoTag == "Invalid Item, Check data table")
		{
			UE_LOG(ErrAsset, Error, TEXT("%s : No such item exist"), *iteminfo.NameTag);
		}

		FItemForm itemform(iteminfo);
		// 썸네일 로드
		itemform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		itemform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		itemform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
		
		FShopItemForm registerform;
		registerform.ItemForm = itemform;
		registerform.Price = ShopItemsInfo[i].Price;
		// 상점 아이템에 추가
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

	if (!bInteracted && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 200.0f)	// 상호작용 중이지 않고, 플레이어가 반경에 들어옴
	{
		bInteractable = true;
		if (Player->GetCurrentAction() == ECustomActionMode::INTERACT)	// 플레이어가 상호작용 키를 누름
		{
			Interact();	// 상호작용
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

	if (prop == "NameTag" || prop == "TypeTag")	// 아이템의 이름이나, 타입이 바뀌었을 경우
	{
		FString name = e.Property->GetName();
		int32 index = e.GetArrayIndex(TEXT("ShopItemsInfo"));	// ShopItemsInfo의 프로퍼티가 바뀌었을 경우, 인덱스 반환
		int code = -1;
		if (index >= 0)
		{
			// 현재 프로퍼티에 맞는 아이템의 정보를 로드 및 프로퍼티 창에 출력
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
			index = e.GetArrayIndex(TEXT("AcquireItemsInfo"));	// AcquireItemsInfo의 프로퍼티가 바뀜
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
	Player->bInteracting = true;	// 플레이어를 상호작용 상태로 변경

	bInteracted = true;	// NPC도 상호작용 상태로 변경

	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.0f);	// NPC의 셀프캠으로 렌더 타겟 변경

	AMainGameMode* gamemode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeCurrentWidget(gamemode->DialogueBoxUI);	// 다이얼로그 UI 엶

	Cast<UDialogueBox>(gamemode->DialogueBoxUI)->OpenUI(this);	// 다이얼로그 로드
}

void ANPC::UnInteract()
{
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Player, 1.0f);	// 플레이어의 카메라로 돌아감
	bInteracted = false;

	Player->GameMode->ChangeCurrentWidget(Player->GameMode->MainUI);	// 메인 UI로 변경
	Player->Notify();	// 상호작용 중 쌓인 알림 표시
	Player->bInteracting = false;
}

void ANPC::OpenShop()
{
	AMainGameMode* gamemode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeCurrentWidget(gamemode->ShopUI);	// 상점 UI 엶

	Cast<UShop>(gamemode->ShopUI)->InitShop(this);	// 상점 아이템 로드
}

void ANPC::GenerateShopItems()
{
	ShopItems.Empty();	// BeginPlay()를 통해 자동 생성된 아이템 목록 비움

	// 세이브 파일에 의해 ShopItemsInfo가 바뀜. 이를 적용
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