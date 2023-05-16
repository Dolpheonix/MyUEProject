// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueButton.h"

UDialogueButton::UDialogueButton()
{
	index = -1;
	SetPressMethod(EButtonPressMethod::DownAndUp);
	OnClicked.AddDynamic(this, &UDialogueButton::ClickTrigger);
}

UDialogueButton::UDialogueButton(int i)
{
	index = i;
	SetPressMethod(EButtonPressMethod::DownAndUp);
	OnClicked.AddDynamic(this, &UDialogueButton::ClickTrigger);
}

void UDialogueButton::ClickTrigger()
{
	DialogueButtonEvent.Broadcast(index);
}
