// Character - NPC
// 플레이어와 상호작용 할 수 있는 캐릭터입니다.
// 대화 및 퀘스트, 상점 기능 구현
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
	// 에디터에서 Array 프로퍼티가 변경되었을 경우
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& e) override;

	virtual void OnHurt() override;
	virtual void OnDead() override;

	virtual void Interact() override;
	virtual void UnInteract() override;

	// 상점 인터페이스를 엶
	void OpenShop();
	// 세이브 파일 정보를 통해 상점 아이템 생성
	void GenerateShopItems();

public:
	// NPC를 찍는 카메라. 상호작용 시, 이 카메라로 화면을 전환함
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* SelfCamera;
	// NPC의 AI 컨트롤러
	UPROPERTY(BlueprintReadWrite)
	AAIController* AIController;
	// NPC의 대화 내용을 담은 트리
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FDialogueTree DialogueTree;
	// 메인 플레이어
	UPROPERTY(BlueprintReadWrite)
	AMainCharacter* Player;
	// 상점 아이템 정보 (수정, 저장용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FShopItemShortForm> ShopItemsInfo; // 상점 아이템 정보
	// 보상 아이템 정보 (수정, 저장용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acquisition")
	TArray<FItemShortForm> AcquireItemsInfo; // 상점 외의 보유 아이템 정보
	// 상점 아이템
	TArray<FShopItemForm> ShopItems;
	// 보상 아이템
	TArray<FItemForm> AcquireItems;
	// 상호작용 위치
	FVector InteractionPoint;
	// 상호작용 가능한 상태인가?
	bool bInteractable = false;
	// 상호작용 중인가?
	bool bInteracted = false;

	// 데이터 테이블 (아이템 정보 확인용)
	class UDataTable* ItemTable;
	class UDataTable* WeaponTable;
	class UDataTable* ClothTable;
};
