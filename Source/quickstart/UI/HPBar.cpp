#include "HPBar.h"

void UHPBar::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	HP_ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	if(HP_ProgressBar) HP_ProgressBar->SetFillColorAndOpacity(FLinearColor::Red);
}