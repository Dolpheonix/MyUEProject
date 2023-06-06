#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Sound/SoundCue.h"
#include "PopUp.generated.h"

UCLASS()
class QUICKSTART_API UPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	void InitializeNumber(int max);
	void Refresh();

	UFUNCTION()
	void ApplyInput(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void Increase();
	UFUNCTION()
	void Decrease();
	UFUNCTION()
	void Confirm();
	UFUNCTION()
	void Cancle();

public:
	UPROPERTY(BlueprintReadWrite)
	UButton* IncreaseButton;

	UPROPERTY(BlueprintReadWrite)
	UButton* DecreaseButton;

	UPROPERTY(BlueprintReadWrite)
	UButton* ConfirmButton;

	UPROPERTY(BlueprintReadWrite)
	UButton* CancleButton;

	UPROPERTY(BlueprintReadWrite)
	UEditableTextBox* NumberInput;

	USoundCue* IncreaseSound;
	USoundCue* DecreaseSound;
	USoundCue* ConfirmSound;
	USoundCue* CancleSound;

	bool Bounded = false;

	int CurrNum = -1;
	int MaxNum = -1;
};