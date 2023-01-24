// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemButton.h"

UItemButton::UItemButton()
{
	index = -1;
	SetPressMethod(EButtonPressMethod::DownAndUp);
}

UItemButton::UItemButton(int i)
{
	index = i;
}

void UItemButton::SetEvent()
{
	OnHovered.AddDynamic(this, &ThisClass::HoverTrigger);
	OnClicked.AddDynamic(this, &ThisClass::ClickTrigger);
	OnPressed.AddDynamic(this, &ThisClass::PressTrigger);

}

void UItemButton::HoverTrigger()
{
	HoveredEvent.Broadcast(index, type);
}

void UItemButton::ClickTrigger()
{
	ClickedEvent.Broadcast(index, type);
}

void UItemButton::PressTrigger()
{
	PressedEvent.Broadcast(index, type);
}
