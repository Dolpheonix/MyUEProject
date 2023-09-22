// UI - Dialogue Box
// NPC���� ��ȣ�ۿ� �� ������ ��ȭ ����
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "../Character/NPC/NPC.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "DialogueButton.h"
#include "DialogueBox.generated.h"

UCLASS()
class QUICKSTART_API UDialogueBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// NPC�κ��� ���̾�α׸� �޾� �ʱ�ȭ
	void OpenUI(ANPC* interacted);
	// index�� �´� ���̾�α׷� ������Ʈ
	void GetDialogue(int index);
	// index�� �´� ����Ʈ ���̾�α׸� ������Ʈ
	void GetQuestDialogue(int index);
	// ���� ���õ� ���̾�α׷� ��ȭ ���ڸ� ������Ʈ
	void RefreshDialogue();
	// ���� ��ư�� ���� ���� ���̾�α׷� �̵�
	UFUNCTION()
	void OnPressed_EndLine(int index);

	// ��ȣ�ۿ����� �÷��̾�
	AMainCharacter* Player;
	// �÷��̾��� ��Ʈ�ѷ�
	APlayerController* Controller;
	// ��ȣ�ۿ����� NPC
	ANPC* InteractedNPC;

	// ��ȭ�� �޾ƿ��� ���� ����Ʈ ������Ʈ
	FQuest* TriggeredQuest;
	// ����Ʈ�� �ε���
	int QuestIndex;

	// ���� ���õ� ���̾�α�
	FDialogueLine DialogueLine;
	// ���� ���õ� ����Ʈ ���̾�α�
	FQuestDialogueLine QuestDialogueLine;

	// ���� ����� Dialogue�� Quest Dialogue/NPC Dialogue
	bool IsQuestDialogue;

	// ���̾�α׸� ����ϴ� �ؽ�Ʈ ���
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* DialogueText;
	// ���̾�α��� ������ ����ϴ� �ؽ�Ʈ ���
	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> ResponseTexts;
	// ���� ���� ��ư
	UPROPERTY(BlueprintReadWrite)
	TArray<UDialogueButton*> ResponseButtons;

	// PreConstruct()�� ó�� �ҷ��� ���� ������ �ʱ�ȭ�ϱ� ���� ���
	bool Bounded = false;
};
