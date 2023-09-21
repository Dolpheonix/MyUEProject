// Interactable - Collectable Item
// �÷��̾ ��ȣ�ۿ��� ���� ������ �� �ִ� ������
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
	// ������ �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* ItemMesh;
	// ������ ������ �� ������ FX
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* AuraComponent;
	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
	FItemShortForm ItemInfo;

	// ������ ������
	class UDataTable* ItemTable;
	class UDataTable* WeaponTable;
	class UDataTable* ClothTable;
};
