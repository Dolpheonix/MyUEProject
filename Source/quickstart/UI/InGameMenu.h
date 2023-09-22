// UI - In Game Menu
// 게임 플레이 중의 메뉴 UI
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "InGameMenu.generated.h"

UCLASS()
class QUICKSTART_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
public:
	// 게임 플레이로 돌아감
	UFUNCTION()
	void ReturnToGame();
	// 현재 상태를 저장
	UFUNCTION()
	void SaveGame();
	// 인트로 화면으로 돌아감
	UFUNCTION()
	void ReturnToIntro();

public:
	UButton* BacktoGameButton;
	UButton* SaveGameButton;
	UButton* IntroButton;

	// PreConstruct()가 처음 불렸을 때만 변수를 초기화하기 위해 사용
	bool bBounded = false;
};
