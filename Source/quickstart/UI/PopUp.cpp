#include "PopUp.h"
#include "Kismet/GameplayStatics.h"
#include "../Utils/Helpers.h"

void UPopUp::NativePreConstruct()
{
	Super::NativePreConstruct();
	bIsFocusable = true;
	if (!Bounded)
	{
		IncreaseSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Abstract_Pop_05_Cue.Abstract_Pop_05_Cue"));
		DecreaseSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Abstract_Pop_02_Cue.Abstract_Pop_02_Cue"));
		ConfirmSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Item_Sell_Purchase_03_Cue.Item_Sell_Purchase_03_Cue"));
		CancleSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/Interface_And_Item_Sounds/Cues/Futuristic_Click_07_Cue.Futuristic_Click_07_Cue"));

		IncreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("Increase")));
		DecreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("Decrease")));
		ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("Confirm")));
		CancleButton = Cast<UButton>(GetWidgetFromName(TEXT("Cancle")));

		NumberInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Number_Input")));

		IncreaseButton->OnPressed.AddDynamic(this, &UPopUp::Increase);
		DecreaseButton->OnPressed.AddDynamic(this, &UPopUp::Decrease);
		ConfirmButton->OnPressed.AddDynamic(this, &UPopUp::Confirm);
		CancleButton->OnPressed.AddDynamic(this, &UPopUp::Cancle);
		NumberInput->OnTextCommitted.AddDynamic(this, &UPopUp::ApplyInput);

		Bounded = true;
	}
}

void UPopUp::InitializeNumber(int max)
{
	CurrNum = 1;
	MaxNum = max;
	NumberInput->SetText(FText::FromString(FString::FromInt(CurrNum)));

	Refresh();
}

void UPopUp::Refresh()
{
	if (CurrNum < 2)
	{
		DecreaseButton->SetIsEnabled(false);
	}
	else
	{
		DecreaseButton->SetIsEnabled(true);
	}

	if (CurrNum >= MaxNum)
	{
		IncreaseButton->SetIsEnabled(false);
	}
	else
	{
		IncreaseButton->SetIsEnabled(true);
	}
}

void UPopUp::Increase()
{
	CurrNum++;
	NumberInput->SetText(FText::FromString(FString::FromInt(CurrNum)));
	Refresh();

	UGameplayStatics::PlaySound2D(this, IncreaseSound);
}

void UPopUp::Decrease()
{
	CurrNum--;
	NumberInput->SetText(FText::FromString(FString::FromInt(CurrNum)));
	Refresh();

	UGameplayStatics::PlaySound2D(this, DecreaseSound);
}

// Just playing sound. Add additional event to ConfimButton in ParentUI
void UPopUp::Confirm()
{
	UGameplayStatics::PlaySound2D(this, ConfirmSound);
}

// Remove from parent
void UPopUp::Cancle()
{
	UGameplayStatics::PlaySound2D(this, CancleSound);
	RemoveFromParent();
}

void UPopUp::ApplyInput(const FText& Text, ETextCommit::Type CommitMethod)
{
	FString str = Text.ToString();
	if (str.IsNumeric())
	{
		int32 num = FCString::Atoi(*str);
		CurrNum = num;
		Refresh();
	}
	else
	{
		NumberInput->SetText(FText::FromString(FString::FromInt(CurrNum)));
	}
}