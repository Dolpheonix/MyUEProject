// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Kismet/GameplayStatics.h"
#include "../Core/GameInstance/MainGameInstance.h"
#include "../Character/Main/MainCharacter.h"

void UInGameMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!bBounded)
	{
		BacktoGameButton = Cast<UButton>(GetWidgetFromName("BacktoGame"));
		if (BacktoGameButton) BacktoGameButton->OnClicked.AddDynamic(this, &UInGameMenu::ReturnToGame);

		SaveGameButton = Cast<UButton>(GetWidgetFromName("SaveGame"));
		if (SaveGameButton) SaveGameButton->OnClicked.AddDynamic(this, &UInGameMenu::SaveGame);

		IntroButton = Cast<UButton>(GetWidgetFromName(TEXT("Intro")));
		if (IntroButton) IntroButton->OnClicked.AddDynamic(this, &UInGameMenu::ReturnToIntro);
		
		bBounded = true;
	}
}

void UInGameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	check(BacktoGameButton);
	check(SaveGameButton);
	check(IntroButton);

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller)
	{
		int X, Y;
		Controller->GetViewportSize(X, Y);
		Controller->SetMouseLocation(X / 2, Y / 2);
		Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
		Controller->SetShowMouseCursor(true);
		Controller->SetInputMode(FInputModeUIOnly());
	}
}

void UInGameMenu::NativeDestruct()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller)
	{
		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
	}

	Super::NativeDestruct();
}

FReply UInGameMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	int ascii = InKeyEvent.GetCharacter();
	if (ascii == 59)
	{
		ReturnToGame();
	}

	return FReply::Handled();
}

void UInGameMenu::ReturnToGame()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller)
	{
		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
	}

	AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Player->GameMode->ChangeCurrentWidget(Player->GameMode->MainUI);
}

void UInGameMenu::SaveGame()
{
	UMainGameInstance* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		GI->SaveCharacterMemory(Player);
		GI->SaveToFile();
	}
}

void UInGameMenu::ReturnToIntro()
{
	UMainGameInstance* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->LoadLevel("Intro_Empty", false);
	}
}