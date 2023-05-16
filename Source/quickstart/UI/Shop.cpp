// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/Main/MainCharacter.h"

void UShop::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();

	if (!Bounded)
	{
		EndButton = Cast<UButton>(GetWidgetFromName(TEXT("End")));
		EndButton->OnClicked.AddDynamic(this, &UShop::EndShop);
	}
}

void UShop::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}

void UShop::NativeDestruct()
{
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());

	InteractedNPC = nullptr;

	UUserWidget::NativeDestruct();
}

void UShop::InitShop(ANPC* Interacted)
{
	auto Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Controller = Cast<APlayerController>(Player->GetController());

	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeUIOnly());

	InteractedNPC = Interacted;
}

void UShop::EndShop()
{
	InteractedNPC->UnInteract();
}