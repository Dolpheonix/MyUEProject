// Copyright Epic Games, Inc. All Rights Reserved.


#include "quickstartGameModeBase.h"
#include <Blueprint/UserWidget.h>
#include "Data/DataTables.h"
#include "Utils/Helpers.h"

AquickstartGameModeBase::AquickstartGameModeBase()
{
    ClothTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Cloth_Sheet.Cloth_Sheet"));
    WeaponTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Weapon_Sheet.Weapon_Sheet"));
    ItemTable = Helpers::C_LoadObjectFromPath<UDataTable>(TEXT("/Game/ShootingGame/Data/Item_Sheet.Item_Sheet"));
}

void AquickstartGameModeBase::BeginPlay()
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

void AquickstartGameModeBase::StartPlay()
{
    Super::StartPlay();
}

void AquickstartGameModeBase::ChangeMenuWidget(UUserWidget* TargetUI)
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

void AquickstartGameModeBase::SniperMode(bool turnOn)
{
    if (turnOn) SniperUI->AddToViewport();
    else SniperUI->RemoveFromViewport();
}