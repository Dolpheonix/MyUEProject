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

    // 지정된 UI 클래스 기반으로 UI 생성
    MainUI = CreateWidget(GetWorld(), MainWidgetClass);
    InGameMenuUI = CreateWidget(GetWorld(), InGameMenuWidgetClass);
    ShowroomUI = CreateWidget(GetWorld(), ShowroomWidgetClass);
    SniperUI = CreateWidget(GetWorld(), SniperWidgetClass);
    DialogueBoxUI = CreateWidget(GetWorld(), DialogueBoxWidgetClass);
    ShopUI = CreateWidget(GetWorld(), ShopWidgetClass);
    QuestUI = CreateWidget(GetWorld(), QuestWidgetClass);

    // 인게임 위젯으로 초기 설정
    ChangeCurrentWidget(MainUI);
}

void AMainGameMode::ChangeCurrentWidget(UUserWidget* TargetUI)
{
    if (CurrentUI != nullptr)   // 현재 UI가 존재한다면 미리 뷰포트에서 제거
    {
        CurrentUI->RemoveFromViewport();
        CurrentUI = nullptr;
    }

    if (TargetUI != nullptr)    // 타켓 UI를 뷰포트에 추가 후 현재 UI로 지정
    {
        CurrentUI = TargetUI;
        CurrentUI->AddToViewport();
    }
}

void AMainGameMode::SniperMode(bool turnOn)
{
    // 조준 모드는 기존의 인게임 UI를 제거하지 않고 추가만 함
    if (turnOn) SniperUI->AddToViewport();
    else SniperUI->RemoveFromViewport();
}