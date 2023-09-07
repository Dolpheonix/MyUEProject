// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/Main/MainCharacter.h"

void UMainWidget::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();
	bIsFocusable = true;
	if (!Bounded)
	{
		HPBarWidget = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
		MinimapImage = Cast<UImage>(GetWidgetFromName(TEXT("Minimap")));
		QuickslotImage_Weapon.Add(Cast<UImage>(GetWidgetFromName("Weapon_Curr")));
		QuickslotImage_Weapon.Add(Cast<UImage>(GetWidgetFromName("Weapon_Next")));
		QuickslotImage_Item.Add(Cast<UImage>(GetWidgetFromName("Item_Curr")));
		QuickslotImage_Item.Add(Cast<UImage>(GetWidgetFromName("Item_Next")));
		NotifyInteractImage = Cast<UImage>(GetWidgetFromName("NotifyInteract"));
		NotifyInteractTextBlock = Cast<UTextBlock>(GetWidgetFromName("NotifyInteractText"));

		Bounded = true;
	}
}

void UMainWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	check(HPBarWidget)
	check(MinimapImage)
	check(QuickslotImage_Item[0])
	check(QuickslotImage_Item[1])
	check(QuickslotImage_Weapon[0])
	check(QuickslotImage_Weapon[1])
	check(NotifyInteractImage)
	check(NotifyInteractTextBlock)
}

void UMainWidget::RefreshHPBar()
{
	AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	HPBarWidget->SetPercent(player->HP / 100);
}

void UMainWidget::RefreshQuickslot()
{
	AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	QuickslotImage_Weapon[0]->SetBrushFromTexture(player->Inventory[(uint8)ETypeTag::Weapon].ItemForms[player->Quickslots_Now[(uint8)ETypeTag::Weapon]].Thumbnail_N);
	QuickslotImage_Weapon[1]->SetBrushFromTexture(player->Inventory[(uint8)ETypeTag::Weapon].ItemForms[player->Quickslots_Next[(uint8)ETypeTag::Weapon]].Thumbnail_N);
	QuickslotImage_Item[0]->SetBrushFromTexture(player->Inventory[(uint8)ETypeTag::Item].ItemForms[player->Quickslots_Now[(uint8)ETypeTag::Item]].Thumbnail_N);
	QuickslotImage_Item[1]->SetBrushFromTexture(player->Inventory[(uint8)ETypeTag::Item].ItemForms[player->Quickslots_Next[(uint8)ETypeTag::Item]].Thumbnail_N);
}