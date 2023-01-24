// Fill out your copyright notice in the Description page of Project Settings.


#include "PreviewActor.h"
#include "Kismet/GameplayStatics.h"
#include "../Utils/Helpers.h"
#include "../Data/DataTables.h"

// Sets default values
APreviewActor::APreviewActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	MainMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Preview Body"));
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	auto ClothNames = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Cloth_Sheet.Cloth_Sheet"))->GetRowNames();
	auto WeaponNames = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Weapon_Sheet.Weapon_Sheet"))->GetRowNames();

	for (int i = 0; i < ClothNames.Num(); i++)
	{
		UStaticMeshComponent* cloth = CreateDefaultSubobject<UStaticMeshComponent>(ClothNames[i]);
		FString name = ClothNames[i].ToString();
		name[0] = toupper(name[0]);
		cloth->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName(name)));
		cloth->SetVisibility(false);
		PreviewClothes.Add(name, cloth);
	}
	
	for (int i = 1; i < WeaponNames.Num(); i++)
	{
		UStaticMeshComponent* weapon = CreateDefaultSubobject<UStaticMeshComponent>(WeaponNames[i]);
		FString name = WeaponNames[i].ToString();
		name[0] = toupper(name[0]);
		weapon->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName(name)));
		weapon->SetupAttachment(RootComponent);
		weapon->SetVisibility(false);
		PreviewWeapons.Add(name, weapon);
	}

	PreviewCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PreviewCamera"));
	PreviewCamera->TextureTarget = Helpers::C_LoadObjectFromPath<UTextureRenderTarget2D>(TEXT("/Game/ShootingGame/UI/Preview.Preview"));
	PreviewCamera->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
	Helpers::SetComponent<USceneCaptureComponent2D>(&PreviewCamera, RootComponent, FVector(50.0f, 0.0f, 0.0f), FRotator(0.0f, 180.0f, 0.0f));
}

// Called when the game starts or when spawned
void APreviewActor::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	SyncEvent.AddDynamic(this, &APreviewActor::Sync_to_Character);
}

// Called every frame
void APreviewActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APreviewActor::Sync_to_Character()
{
	Weapon_Now = Player->Weapon_Now;
	for (int i = 0; i < Player->Weapons.Num(); i++)
	{
		if (i == Weapon_Now)
		{
			auto tag = Player->Weapons[i].NameTag;
			if (tag != "Fist")
			{
				auto weapon = PreviewWeapons[tag];
				weapon->AttachToComponent(MainMesh, { EAttachmentRule::SnapToTarget, true }, FName(tag + "_Equip"));
				weapon->SetVisibility(true);
			}
			WeaponCode = Player->WeaponCode;
		}
		else
		{
			auto tag = Player->Weapons[i].NameTag;
			if (tag != "Fist")
			{
				auto weapon = PreviewWeapons[tag];
				weapon->AttachToComponent(MainMesh, { EAttachmentRule::SnapToTarget, true }, FName(tag + "_unEquip"));
				weapon->SetVisibility(true);
			}
		}
	}

	Cloth_Now = Player->Cloth_Now;
	for (int i = 0; i < Player->Clothes.Num(); i++)
	{
	}
}

void APreviewActor::Delete_and_Sync(ETypeTag type, FString tag)
{
	if (type == ETypeTag::Weapon)
	{
		auto toHide = PreviewWeapons[tag];
		toHide->SetVisibility(false);
	}
	else if(type == ETypeTag::Cloth)
	{

	}

	Sync_to_Character();
}
