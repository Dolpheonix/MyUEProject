// Interactable - Collectable Item
// 플레이어가 상호작용을 통해 루팅할 수 있는 아이템
#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interactable.h"
#include "CollectableItem.generated.h"

UCLASS()
class QUICKSTART_API ACollectableItem : public AInteractable
{
	GENERATED_BODY()
	
public:
	ACollectableItem();

protected:
	virtual void BeginPlay() override;
	virtual void Interact() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	// Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* MeshComponent;
	// 아이템 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* ItemMesh;
	// 범위에 들어왔을 때 켜지는 FX
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* AuraComponent;
	// 아이템 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
	FItemShortForm ItemInfo;

	// 아이템 데이터
	class UDataTable* ItemTable;
	class UDataTable* WeaponTable;
	class UDataTable* ClothTable;
};
