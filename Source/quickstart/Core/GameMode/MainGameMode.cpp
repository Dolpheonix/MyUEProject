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
}

void AMainGameMode::BeginPlay()
{
    Super::BeginPlay();

    // ������ UI Ŭ���� ������� UI ����
    MainUI = CreateWidget(GetWorld(), MainWidgetClass);
    InGameMenuUI = CreateWidget(GetWorld(), InGameMenuWidgetClass);
    ShowroomUI = CreateWidget(GetWorld(), ShowroomWidgetClass);
    SniperUI = CreateWidget(GetWorld(), SniperWidgetClass);
    DialogueBoxUI = CreateWidget(GetWorld(), DialogueBoxWidgetClass);
    ShopUI = CreateWidget(GetWorld(), ShopWidgetClass);
    QuestUI = CreateWidget(GetWorld(), QuestWidgetClass);

    // �ΰ��� �������� �ʱ� ����
    ChangeCurrentWidget(MainUI);
}

void AMainGameMode::ChangeCurrentWidget(UUserWidget* TargetUI)
{
    if (CurrentUI != nullptr)   // ���� UI�� �����Ѵٸ� �̸� ����Ʈ���� ����
    {
        CurrentUI->RemoveFromViewport();
        CurrentUI = nullptr;
    }

    if (TargetUI != nullptr)    // Ÿ�� UI�� ����Ʈ�� �߰� �� ���� UI�� ����
    {
        CurrentUI = TargetUI;
        CurrentUI->AddToViewport();
    }
}

void AMainGameMode::SniperMode(bool turnOn)
{
    // ���� ���� ������ �ΰ��� UI�� �������� �ʰ� �߰��� ��
    if (turnOn) SniperUI->AddToViewport();
    else SniperUI->RemoveFromViewport();
}