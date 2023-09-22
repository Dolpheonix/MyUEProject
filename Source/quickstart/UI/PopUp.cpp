#include "PopUp.h"
#include "Kismet/GameplayStatics.h"
#include "../Utils/Helpers.h"

void UPopUp::NativePreConstruct()
{
	Super::NativePreConstruct();

	bIsFocusable = true;
	if (!Bounded)
	{
		IncreaseSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Pop_03_Cue.Pop_03_Cue"));
		DecreaseSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Pop_02_Cue.Pop_02_Cue"));
		ConfirmSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Item_Sell_Purchase_01_Cue.Item_Sell_Purchase_01_Cue"));
		CancleSound = Helpers::LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Click_02_Cue.Click_02_Cue"));

		IncreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("Increase")));
		DecreaseButton = Cast<UButton>(GetWidgetFromName(TEXT("Decrease")));
		ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("Confirm")));
		CancleButton = Cast<UButton>(GetWidgetFromName(TEXT("Cancle")));

		NumberInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Number_Input")));

		if(IncreaseButton) IncreaseButton->OnPressed.AddDynamic(this, &UPopUp::Increase);
		if(DecreaseButton) DecreaseButton->OnPressed.AddDynamic(this, &UPopUp::Decrease);
		if(ConfirmButton) ConfirmButton->OnPressed.AddDynamic(this, &UPopUp::Confirm);
		if(CancleButton) CancleButton->OnPressed.AddDynamic(this, &UPopUp::Cancle);
		if(NumberInput) NumberInput->OnTextCommitted.AddDynamic(this, &UPopUp::ApplyInput);

		Bounded = true;
	}
}

void UPopUp::InitializeNumber(int max)
{
	check(IncreaseButton && DecreaseButton && ConfirmButton && CancleButton && NumberInput);

	// CurrNum은 1, MaxNum은 선택된 아이템의 개수로 설정
	CurrNum = 1;
	MaxNum = max;
	NumberInput->SetText(FText::FromString(FString::FromInt(CurrNum)));	// 숫자 입력란에 CurrNum 적용

	Refresh();
}

void UPopUp::Refresh()
{
	if (CurrNum < 2)	// 더 줄일 수 없음
	{
		DecreaseButton->SetIsEnabled(false);
	}
	else
	{
		DecreaseButton->SetIsEnabled(true);
	}

	if (CurrNum >= MaxNum)	// 더 늘릴 수 없음
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

void UPopUp::Confirm()
{
	UGameplayStatics::PlaySound2D(this, ConfirmSound);
}

void UPopUp::Cancle()
{
	UGameplayStatics::PlaySound2D(this, CancleSound);
	RemoveFromParent();	// 팝업창 삭제
}

void UPopUp::ApplyInput(const FText& Text, ETextCommit::Type CommitMethod)
{
	FString str = Text.ToString();
	if (str.IsNumeric())	// 입력이 숫자이므로 유효 ==> CurrNum에 적용
	{
		int32 num = FCString::Atoi(*str);
		CurrNum = num;
		Refresh();
	}
	else	// 입력이 숫자가 아니므로 다시 원래대로 돌려놓음
	{
		NumberInput->SetText(FText::FromString(FString::FromInt(CurrNum)));
	}
}