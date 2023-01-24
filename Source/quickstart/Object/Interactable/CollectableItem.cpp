// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItem.h"
#include "Kismet/GameplayStatics.h"
#include "../../quickstart.h"
#include "../../quickstartGameModeBase.h"
#include "../../Data/DataTables.h"


ACollectableItem::ACollectableItem()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	AuraComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	AuraComponent->SetupAttachment(RootComponent);
	AuraComponent->SetTemplate(Helpers::C_LoadObjectFromPath<UParticleSystem>(TEXT("/Game/FXVarietyPack/Particles/P_ky_healAura.P_ky_healAura")));
	AuraComponent->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	AuraComponent->Deactivate();
}

void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();

	Forward = GetActorForwardVector();
	InteractPoint += MeshComponent->GetRelativeLocation();

	auto gameMode = Cast<AquickstartGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	switch (TypeTag)
	{
	case ETypeTag::Cloth:
	{
		auto table = gameMode->ClothTable;
		FItemTableRow* row = table->FindRow<FItemTableRow>(*NameTag, "");
		if (row)
		{
			InfoTag = row->Info;
		}
		else
		{
			UE_LOG(ErrDataTable, Error, TEXT("%s | Invalid Row : %s"), *GetName(), *NameTag);
		}
		break;
	}
	case ETypeTag::Weapon:
	{
		auto table = gameMode->WeaponTable;
		FWeaponTableRow* row = table->FindRow<FWeaponTableRow>(*NameTag, "");
		if (row)
		{
			InfoTag = row->Info;
			WeaponCode = row->Code;
		}
		else
		{
			UE_LOG(ErrDataTable, Error, TEXT("%s | Invalid Row : %s"), *GetName(), *NameTag);
		}
		break;
	}
	case ETypeTag::Item:
	{
		auto table = gameMode->ItemTable;
		FItemTableRow* row = table->FindRow<FItemTableRow>(*NameTag, "");
		if (row)
		{
			InfoTag = row->Info;
		}
		else
		{
			UE_LOG(ErrDataTable, Error, TEXT("%s | Invalid Row : %s"), *GetName(), *NameTag);
		}
		break;
	}
	default:
		break;
	}

	if (ItemMesh) MeshComponent->SetStaticMesh(ItemMesh);
}

void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInteractable)
	{
		AuraComponent->Activate();
	}
	else
	{
		AuraComponent->Deactivate();
	}

	MeshComponent->AddRelativeRotation(FRotator(0.0f, 0.5f, 0.0f));
}

void ACollectableItem::Interact()
{
	Player->Register(TypeTag, NameTag, InfoTag, WeaponCode);
	Destroy();
}
