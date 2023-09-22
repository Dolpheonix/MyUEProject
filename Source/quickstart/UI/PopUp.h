// UI - Popup
// �κ��丮 UI���� ������ ���� ��, ���� ���� �� ���� Ȯ�ο� �˾�â
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

	// ���� �ʱ�ȭ
	void InitializeNumber(int max);
	// CurrNum�� ���� ����/���� ��ư�� Ȱ��ȭ ���θ� ����
	void Refresh();
	// ���� �Է�â�� ������ �Է����� ��, �̸� ����
	UFUNCTION()
	void ApplyInput(const FText& Text, ETextCommit::Type CommitMethod);
	// ������ ���� ����
	UFUNCTION()
	void Increase();
	// ������ ���� ����
	UFUNCTION()
	void Decrease();
	// Ȯ�� ��ư ���� ��
	UFUNCTION()
	void Confirm();
	// ��� ��ư ���� ��
	UFUNCTION()
	void Cancle();

public:
	// ���� ��ư
	UPROPERTY(BlueprintReadWrite)
	UButton* IncreaseButton;
	// ���� ��ư
	UPROPERTY(BlueprintReadWrite)
	UButton* DecreaseButton;
	// Ȯ�� ��ư
	UPROPERTY(BlueprintReadWrite)
	UButton* ConfirmButton;
	// ��� ��ư
	UPROPERTY(BlueprintReadWrite)
	UButton* CancleButton;
	// ���� �Է�â
	UPROPERTY(BlueprintReadWrite)
	UEditableTextBox* NumberInput;
	
	// ȿ����
	USoundCue* IncreaseSound;
	USoundCue* DecreaseSound;
	USoundCue* ConfirmSound;
	USoundCue* CancleSound;

	// PreConstruct()�� ó�� �ҷ��� ���� ������ �ʱ�ȭ�ϱ� ���� ���
	bool Bounded = false;

	// ���� ������ Num
	int CurrNum = -1;
	// �ִ�� ������ �� �ִ� Num (������ �������� ���� ����)
	int MaxNum = -1;
};