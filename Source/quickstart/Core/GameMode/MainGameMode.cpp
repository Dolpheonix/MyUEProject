// Copyright Epic Games, Inc. All Rights Reserved.
#define MAXACTIONCODE 100

#include "MainGameMode.h"
#include <Blueprint/UserWidget.h>
#include "../../Data/DataTables.h"
#include "../../Utils/Helpers.h"

AMainGameMode::AMainGameMode()
{
    ClothTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Cloth_Sheet.Cloth_Sheet"));
    WeaponTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Weapon_Sheet.Weapon_Sheet"));
    ItemTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Item_Sheet.Item_Sheet"));

    ActionCodeTable.SetNum(MAXACTIONCODE);
}

void AMainGameMode::BeginPlay()
{
    Super::BeginPlay();
    MainUI = CreateWidget(GetWorld(), MainWidgetClass);
    ShowroomUI = CreateWidget(GetWorld(), ShowroomWidgetClass);
    SniperUI = CreateWidget(GetWorld(), SniperWidgetClass);
    DialogueBoxUI = CreateWidget(GetWorld(), DialogueBoxWidgetClass);
    ShopUI = CreateWidget(GetWorld(), ShopWidgetClass);
    QuestUI = CreateWidget(GetWorld(), QuestWidgetClass);

    ChangeMenuWidget(MainUI);
}

void AMainGameMode::StartPlay()
{
    Super::StartPlay();
}

void AMainGameMode::ChangeMenuWidget(UUserWidget* TargetUI)
{
    if (CurrentUI != nullptr)
    {
        CurrentUI->RemoveFromViewport();
        CurrentUI = nullptr;
    }

    if (TargetUI != nullptr)
    {
        CurrentUI = TargetUI;
        CurrentUI->AddToViewport();
    }
}

void AMainGameMode::SniperMode(bool turnOn)
{
    if (turnOn) SniperUI->AddToViewport();
    else SniperUI->RemoveFromViewport();
}