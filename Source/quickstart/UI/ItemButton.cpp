// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemButton.h"

UItemButton::UItemButton()
{
	Index = -1;
	SetPressMethod(EButtonPressMethod::DownAndUp);
}

UItemButton::UItemButton(int i)
{
	Index = i;
}

void UItemButton::SetEvent()
{
	OnHovered.AddDynamic(this, &ThisClass::HoverTrigger);
	OnClicked.AddDynamic(this, &ThisClass::ClickTrigger);
	OnPressed.AddDynamic(this, &ThisClass::PressTrigger);

}

void UItemButton::HoverTrigger()
{
	HoveredEvent.Broadcast(Index, Type);
}

void UItemButton::ClickTrigger()
{
	ClickedEvent.Broadcast(Index, Type);
}

void UItemButton::PressTrigger()
{
	PressedEvent.Broadcast(Index, Type);
}
