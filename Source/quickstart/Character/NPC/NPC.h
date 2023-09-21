// Character - NPC
// �÷��̾�� ��ȣ�ۿ� �� �� �ִ� ĳ�����Դϴ�.
// ��ȭ �� ����Ʈ, ���� ��� ����
#pragma once

#include "CoreMinimal.h"
#include "../Character_Root.h"
#include "../Main/MainCharacter.h"
#include "../../Interface/InteractionInterface.h"
#include "../../Utils/Structs.h"
#include "../../Utils/NPCUtil.h"
#include "AIController.h"
#include "NPC.generated.h"

UCLASS()
class QUICKSTART_API ANPC : public ACharacter_Root, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	ANPC();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	// �����Ϳ��� Array ������Ƽ�� ����Ǿ��� ���
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& e) override;

	virtual void OnHurt() override;
	virtual void OnDead() override;

	virtual void Interact() override;
	virtual void UnInteract() override;

	// ���� �������̽��� ��
	void OpenShop();
	// ���̺� ���� ������ ���� ���� ������ ����
	void GenerateShopItems();

public:
	// NPC�� ��� ī�޶�. ��ȣ�ۿ� ��, �� ī�޶�� ȭ���� ��ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* SelfCamera;
	// NPC�� AI ��Ʈ�ѷ�
	UPROPERTY(BlueprintReadWrite)
	AAIController* AIController;
	// NPC�� ��ȭ ������ ���� Ʈ��
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FDialogueTree DialogueTree;
	// ���� �÷��̾�
	UPROPERTY(BlueprintReadWrite)
	AMainCharacter* Player;
	// ���� ������ ���� (����, �����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FShopItemShortForm> ShopItemsInfo; // ���� ������ ����
	// ���� ������ ���� (����, �����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acquisition")
	TArray<FItemShortForm> AcquireItemsInfo; // ���� ���� ���� ������ ����
	// ���� ������
	TArray<FShopItemForm> ShopItems;
	// ���� ������
	TArray<FItemForm> AcquireItems;
	// ��ȣ�ۿ� ��ġ
	FVector InteractionPoint;
	// ��ȣ�ۿ� ������ �����ΰ�?
	bool bInteractable = false;
	// ��ȣ�ۿ� ���ΰ�?
	bool bInteracted = false;

	// ������ ���̺� (������ ���� Ȯ�ο�)
	class UDataTable* ItemTable;
	class UDataTable* WeaponTable;
	class UDataTable* ClothTable;
};
