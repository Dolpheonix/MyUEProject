// UI - Inventory
// �÷��̾��� �κ��丮 UI
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ItemButton.h"
#include "../Character/Main/MainCharacter.h"
#include "PopUp.h"
#include "PreviewActor.h"
#include "Sound/SoundCue.h"
#include "Inventory.generated.h"

UCLASS()
class QUICKSTART_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& Mygeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	// ������ ������ ���ε�
	void InitializeSlots();
	// ���� �κ��丮�� �°� ���� ������Ʈ
	void RefreshSlots();
	// ��ư �̺�Ʈ�� ���ε�
	void SetEvents();
	// �˾� â�� ��
	void OpenPopup();
	// �˾� â�� ������ ��, ���� ��ư���� ��Ȱ��ȭ
	UFUNCTION()
	void DisableButtonsTemporarily();
	// �ٽ� ��ư���� Ȱ��ȭ��
	UFUNCTION()
	void RestoreButtons();
	
	// ���콺�� ������ ���� ���� ���� ��, �������� ������ ǥ��
	UFUNCTION()
	void OnHovered_GetInfo(int index, ETypeTag type);
	// ������ ���� ǥ�ø� ����
	UFUNCTION()
	void OnUnhovered_DelInfo();
	// ������ Ŭ������ ��, �ش� �������� ����
	UFUNCTION()
	void OnClicked_Select(int index, ETypeTag type);
	// ������ ������ ��, �ش� ������ ĸ�� (�巡��)
	UFUNCTION()
	void OnPressed_Catch(int index, ETypeTag type);
	// ĸ�ĵ� ������ �����뿡�� Release���� ��, �ش� �������� ����
	UFUNCTION()
	void OnReleased_Delete();
	// ������ ���� Ŭ������ ��, �ش� ������ ���� ��ȯ
	UFUNCTION()
	void OnClicked_Tab(int index, ETypeTag type);
	// �������� ����
	UFUNCTION()
	void ThrowAway();
	// �κ��丮�� ����
	UFUNCTION()
	void Exit();
	// 
	void ChangeNumber(int index, ETypeTag type);


public:
	UPROPERTY(BlueprintReadWrite)
	UCanvasPanel* RootCanvas = nullptr;
	// ������ ������ ����ϴ� �ؽ�Ʈ ���
	UPROPERTY(BlueprintReadWrite)
	UTextBlock* InfoText;
	// ������ ����
	TArray<UItemButton*> ItemButtons;
	// ������ ����
	TArray<UTextBlock*> ItemNumbers;
	// �κ��丮�� ��
	TArray<UItemButton*> ItemTabs;
	// ���� �ؽ�Ʈ ���
	TArray<UTextBlock*> TabTexts;

	// ���� ���õ� ��
	ETypeTag CurrTab;
	UTextBlock* CurrTabText;

	// ���� ������ Money�� ����ϴ� �ؽ�Ʈ �ҷ�
	UTextBlock* CurrMoneyText;

	// ������ ��ư
	UPROPERTY(BlueprintReadWrite)
	UButton* TrashCan;
	// Exit ��ư
	UPROPERTY(BlueprintReadWrite)
	UButton* ExitButton;
	// �˾� UI�� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PopUp")
	TSubclassOf<UPopUp> PopupClass;
	// �������� �����뿡 ���� ��, Ȯ�ο� �˾�â
	UPopUp* DeletePopup;

	// ������ ���� �� ����Ǵ� ȿ����
	USoundCue* SelectSound;
	// �������� ĸ���� �� ����Ǵ� ȿ����
	USoundCue* CatchSound;
	// �˾� â�� ���� �� ����Ǵ� ȿ����
	USoundCue* PopupSound;
	// UI�� ���� �� ����Ǵ� ȿ����
	USoundCue* ExitSound;

	// ������ ������ �⺻ �̹���
	UTexture2D* DefaultTex;
	FSlateBrush DefaultBrush;
	FButtonStyle DefaultStyle;
	// �⺻ ��Ʈ
	FSlateFontInfo DefaultFont;

	// �÷��̾�
	UPROPERTY(BlueprintReadOnly)
	AMainCharacter* Player;

	UPROPERTY(BlueprintReadOnly)
	APlayerController* Controller;

	// ������ ����
	APreviewActor* Preview;

	// ���� ĸ�� �� �������ϰ� ��Ÿ���� ���� �̹���
	UPROPERTY(BlueprintReadWrite)
	UImage* CapturedImage;
	// ���� ������ ĸ�����ΰ�?
	UPROPERTY(BlueprintReadWrite)
	bool isCaptured = false;
	// ĸ�ĵ� ������ ����
	CapturedSlot_Info CapturedInfo;

	// �˾� â�� ������ ��, ��Ȱ��ȭ�� ��ư�� ���
	TArray<int> ShutdownList;

	// PreConstruct()�� ó�� �ҷ��� ���� ������ �ʱ�ȭ�ϱ� ���� ���
	bool Bounded = false;
};
