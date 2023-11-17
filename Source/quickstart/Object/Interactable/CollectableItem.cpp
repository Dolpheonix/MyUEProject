#include "CollectableItem.h"
#include "Kismet/GameplayStatics.h"
#include "../../quickstart.h"
#include "../../Core/GameMode/MainGameMode.h"
#include "../../Utils/Helpers.h"
#include "../../Data/DataTables.h"

ACollectableItem::ACollectableItem()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	AuraComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	AuraComponent->SetupAttachment(RootComponent);
	AuraComponent->SetTemplate(Helpers::C_LoadObjectFromPath<UParticleSystem>(TEXT("/Game/ShootingGame/Particle/FX_VarietyPack/FX/P_ky_healAura.P_ky_healAura")));
	AuraComponent->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));

	ClothTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Cloth_Sheet.Cloth_Sheet"));
	WeaponTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Weapon_Sheet.Weapon_Sheet"));
	ItemTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Item_Sheet.Item_Sheet"));
}

void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();

	AuraComponent->Deactivate();
}

void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInteractable)	// 상호작용 가능한 상태면
	{
		AuraComponent->Activate();
	}
	else
	{
		AuraComponent->Deactivate();
	}

	MeshComponent->AddRelativeRotation(FRotator(0.0f, 0.5f, 0.0f));	// 틱마다 회전
}
#if WITH_EDITOR
void ACollectableItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString Name = PropertyChangedEvent.Property->GetName();
	if (Name == TEXT("ItemMesh"))
	{
		MeshComponent->SetStaticMesh(ItemMesh);
	}
	else if (Name == TEXT("TypeTag") || Name == TEXT("NameTag"))
	{
		switch (ItemInfo.TypeTag)
		{
		case ETypeTag::Cloth:
		{
			auto table = ClothTable;
			FItemTableRow* row = table->FindRow<FItemTableRow>(*ItemInfo.NameTag, "");
			if (row)
			{
				ItemInfo.InfoTag = row->Info;
				MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(FName(Helpers::GetMeshFromName(ItemInfo.NameTag))));
			}
			else
			{
				ItemInfo.InfoTag = "Invalid Item, Check data table";
				MeshComponent->SetStaticMesh(nullptr);
			}
			break;
		}
		case ETypeTag::Weapon:
		{
			auto table = WeaponTable;
			FWeaponTableRow* row = table->FindRow<FWeaponTableRow>(*ItemInfo.NameTag, "");
			if (row)
			{
				ItemInfo.InfoTag = row->Info;
				ItemInfo.Code = row->Code;
				MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(FName(Helpers::GetMeshFromName(ItemInfo.NameTag))));
			}
			else
			{
				ItemInfo.InfoTag = "Invalid Item, Check data table";
				MeshComponent->SetStaticMesh(nullptr);
			}
			break;
		}
		case ETypeTag::Item:
		{
			auto table = ItemTable;
			FItemTableRow* row = table->FindRow<FItemTableRow>(*ItemInfo.NameTag, "");
			if (row)
			{
				ItemInfo.InfoTag = row->Info;
				MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(FName(Helpers::GetMeshFromName(ItemInfo.NameTag))));
			}
			else
			{
				ItemInfo.InfoTag = "Invalid Item, Check data table";
				MeshComponent->SetStaticMesh(nullptr);
			}
			break;
		}
		default:
			break;
		}
	}
}
#endif
void ACollectableItem::Interact()
{
	Player->Register(ItemInfo);	// 플레이어에 아이템을 추가
	Player->InteractionFlag--;
	Destroy();	// 액터 삭제
}
