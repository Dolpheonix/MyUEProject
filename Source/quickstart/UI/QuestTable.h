// UI - Quest Table
// �÷��̾ �������� ����Ʈ ���� ǥ��
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/Overlay.h"
#include "QuestTable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestButtonEvent, int, index);

// ����Ʈ ���� ��ư
// �ε�����, �ε����� �Ķ���ͷ� ������ Ŭ�� �̺�Ʈ ����
UCLASS()
class QUICKSTART_API UQuestButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	// Ŭ�� �̺�Ʈ
	FQuestButtonEvent ClickedEvent;

	UFUNCTION()
	void FireClickEvent();

public:
	// ���� ��ư
	UPROPERTY(BlueprintReadWrite)
	UButton* QuestSlotButton;
	// ����Ʈ �̸��� ǥ���� �ؽ�Ʈ ���
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* QuestSlotNameText;
	// ����Ʈ�� �ε��� (�÷��̾�� ��ϵ� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Index")
	int Index;

	// PreConstruct()�� ó�� �ҷ��� ���� ������ �ʱ�ȭ�ϱ� ���� ���
	bool Bounded = false;
};

UCLASS()
class QUICKSTART_API UQuestTable : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	// ����Ʈ UI�� ��
	void InitQuestUI(AActor* Instigator);
	// ����Ʈ ������ ���ΰ�ħ
	void RefreshBlocks();
	// ���õ� ����Ʈ�� ������ �ҷ���
	UFUNCTION()
	void OpenQuestInfo(int index);
	// ��������Ʈ ������ ���
	FString GetSubQuestText(FSingleQuest* subquest);

public:
	AMainCharacter* Player;
	APlayerController* Controller;

	// ����Ʈ ������ ��Ƴ��� ��ũ�� �ڽ�
	UScrollBox* QuestSlotsScroll;
	// ����Ʈ ���� �迭
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UQuestButton*> QuestSlotButtons;
	// ����Ʈ ������ ��Ƴ��� ���� �г�
	UVerticalBox* QuestInfoBox;
	// ����Ʈ �̸� �ؽ�Ʈ
	UTextBlock* QuestNameText;
	// ����Ʈ ���� �ؽ�Ʈ
	UTextBlock* QuestInfoText;
	// ��������Ʈ ���� �ؽ�Ʈ
	UTextBlock* SubQuestsText;
	
	// ���õ� ������ �ε���
	int SelectedSlotIndex = -1;

	// PreConstruct()�� ó�� �ҷ��� ���� ������ �ʱ�ȭ�ϱ� ���� ���
	bool Bounded = false;
};
