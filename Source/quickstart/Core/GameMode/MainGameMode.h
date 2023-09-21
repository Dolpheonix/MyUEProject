///////////////////Main Game Mode//////////////////////
// ���� ���� ���
// 1. �ֿ� UI Ŭ���� ���� �� ����
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

	// ���� ������ ����Ʈ���� �����ϰ� ���ο� ������ �߰�
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeCurrentWidget(UUserWidget* TargetUI);
	// ���� ��� Ȱ��ȭ/��Ȱ��ȭ
	UFUNCTION()
	void SniperMode(bool turnOn);

protected:
	virtual void BeginPlay() override;

public:
	// �ΰ��� UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> MainWidgetClass;
	// �⺻ �޴� UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> InGameMenuWidgetClass;
	// �κ��丮 UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> ShowroomWidgetClass;
	// ���ظ�� UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> SniperWidgetClass;
	// ��ȭ���� UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> DialogueBoxWidgetClass;
	// ���� UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> ShopWidgetClass;
	// ����Ʈ UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> QuestWidgetClass;
	// �˸� UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> NotifyWidgetClass;
	
	// �ΰ��� UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* MainUI;
	// �⺻ �޴� UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* InGameMenuUI;
	// �κ��丮 UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ShowroomUI;
	// ���ظ�� UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* SniperUI;
	// ��ȭ���� UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* DialogueBoxUI;
	// ���� UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ShopUI;
	// ����Ʈ UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* QuestUI;
	
	// ���� UI
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* CurrentUI;

	// ������ Data Table
	class UDataTable* ItemTable;
	class UDataTable* WeaponTable;
	class UDataTable* ClothTable;
};
