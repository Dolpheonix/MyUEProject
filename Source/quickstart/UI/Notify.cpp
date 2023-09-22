#include "Notify.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "../Utils/Helpers.h"

void UNotify::NativePreConstruct()
{
	Super::NativePreConstruct();

	bIsFocusable = true;
	if (!Bounded)
	{
		NotifyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Notify")));
		
		ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("Confirm")));
		if (ConfirmButton) ConfirmButton->OnPressed.AddDynamic(this, &UNotify::OnPressed_Confirm);
	}
}

void UNotify::NativeConstruct()
{
	Super::NativeConstruct();
	check(NotifyText);
	check(ConfirmButton);
}

void UNotify::SetNotification(FString str)
{
	NotifyText->SetText(FText::FromString(str));
}

void UNotify::OnPressed_Confirm()
{
	RemoveFromParent();
}

void UNotifyAcquire::InitializeNotifications(TArray<FReward> notifications)
{
	ACharacter* Player = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	APlayerController* Controller = Cast<APlayerController>(Player->GetController());

	// 화면 중앙에 마우스 커서 표시
	int X, Y;
	Controller->GetViewportSize(X, Y);
	Controller->SetMouseLocation(X / 2, Y / 2);
	Controller->CurrentMouseCursor = EMouseCursor::Crosshairs;
	Controller->SetShowMouseCursor(true);
	Controller->SetInputMode(FInputModeGameAndUI());

	Notifications = notifications;
	// 첫 리워드부터 표시
	currIndex = 0;
	SetNotification();	
}

void UNotifyAcquire::SetNotification()
{
	FString str;
	switch(Notifications[currIndex].Type)
	{
	case ERewardType::EXP:
		str += "GET ";
		str += FString::SanitizeFloat(Notifications[currIndex].EXP);
		str += " EXP!";
		break;
	case ERewardType::ITEM:
		str += "GET ";
		str += FString::FromInt(Notifications[currIndex].Item.Num);
		str += Notifications[currIndex].Item.NameTag;
		str += "!";
		break;
	case ERewardType::MONEY:
		str += "GET ";
		str += FString::FromInt(Notifications[currIndex].Money);
		str += "MONEY!";
		break;
	default:
		break;
	}

	if(NotifyText) NotifyText->SetText(FText::FromString(str));
}

void UNotifyAcquire::OnPressed_Confirm()
{
	currIndex++;
	if (currIndex == Notifications.Num())	// 모든 리워드를 표시했으므로, 알림창 삭제
	{
		ACharacter* Player = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		APlayerController* Controller = Cast<APlayerController>(Player->GetController());

		Controller->SetShowMouseCursor(false);
		Controller->SetInputMode(FInputModeGameOnly());
		RemoveFromParent();
	}
	else
	{
		SetNotification();
	}
}