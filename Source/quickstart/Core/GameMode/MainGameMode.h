///////////////////Main Game Mode//////////////////////
// 메인 게임 모드
// 1. 주요 UI 클래스 변경 및 제거
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../../Utils/NPCUtil.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMainGameMode();

	// 기존 위젯을 뷰포트에서 제거하고 새로운 위젯을 추가
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeCurrentWidget(UUserWidget* TargetUI);
	// 조준 모드 활성화/비활성화
	UFUNCTION()
	void SniperMode(bool turnOn);

protected:
	virtual void BeginPlay() override;

public:
	// 인게임 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> MainWidgetClass;
	// 기본 메뉴 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> InGameMenuWidgetClass;
	// 인벤토리 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> ShowroomWidgetClass;
	// 조준모드 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> SniperWidgetClass;
	// 대화상자 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> DialogueBoxWidgetClass;
	// 상점 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> ShopWidgetClass;
	// 퀘스트 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> QuestWidgetClass;
	// 알림 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> NotifyWidgetClass;
	
	// 인게임 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* MainUI;
	// 기본 메뉴 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* InGameMenuUI;
	// 인벤토리 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ShowroomUI;
	// 조준모드 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* SniperUI;
	// 대화상자 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* DialogueBoxUI;
	// 상점 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ShopUI;
	// 퀘스트 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* QuestUI;
	
	// 현재 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* CurrentUI;

	// 아이템 Data Table
	class UDataTable* ItemTable;
	class UDataTable* WeaponTable;
	class UDataTable* ClothTable;
};
