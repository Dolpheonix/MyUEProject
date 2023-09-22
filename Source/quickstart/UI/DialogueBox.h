// UI - Dialogue Box
// NPC와의 상호작용 시 열리는 대화 상자
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

	// NPC로부터 다이얼로그를 받아 초기화
	void OpenUI(ANPC* interacted);
	// index에 맞는 다이얼로그로 업데이트
	void GetDialogue(int index);
	// index에 맞는 퀘스트 다이얼로그를 업데이트
	void GetQuestDialogue(int index);
	// 현재 선택된 다이얼로그로 대화 상자를 업데이트
	void RefreshDialogue();
	// 선택 버튼을 눌러 다음 다이얼로그로 이동
	UFUNCTION()
	void OnPressed_EndLine(int index);

	// 상호작용중인 플레이어
	AMainCharacter* Player;
	// 플레이어의 컨트롤러
	APlayerController* Controller;
	// 상호작용중인 NPC
	ANPC* InteractedNPC;

	// 대화를 받아오는 중인 퀘스트 오브젝트
	FQuest* TriggeredQuest;
	// 퀘스트의 인덱스
	int QuestIndex;

	// 현재 선택된 다이얼로그
	FDialogueLine DialogueLine;
	// 현재 선택된 퀘스트 다이얼로그
	FQuestDialogueLine QuestDialogueLine;

	// 현재 출력할 Dialogue가 Quest Dialogue/NPC Dialogue
	bool IsQuestDialogue;

	// 다이얼로그를 출력하는 텍스트 블록
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* DialogueText;
	// 다이얼로그의 응답을 출력하는 텍스트 블록
	UPROPERTY(BlueprintReadWrite)
	TArray<UTextBlock*> ResponseTexts;
	// 응답 선택 버튼
	UPROPERTY(BlueprintReadWrite)
	TArray<UDialogueButton*> ResponseButtons;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool Bounded = false;
};
