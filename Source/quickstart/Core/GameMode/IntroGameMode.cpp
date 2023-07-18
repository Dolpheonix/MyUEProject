// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroGameMode.h"

AIntroGameMode::AIntroGameMode()
{

}

void AIntroGameMode::BeginPlay()
{
	Super::BeginPlay();
	IntroUI = CreateWidget(GetWorld(), IntroWidgetClass);

	IntroUI->AddToViewport();
}

void AIntroGameMode::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	IntroUI->RemoveFromViewport();

	Super::EndPlay(EndPlayReason);
}

