// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBar.h"

void UHPBar::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	HP_ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	HP_ProgressBar->SetFillColorAndOpacity(FLinearColor::Red);
}