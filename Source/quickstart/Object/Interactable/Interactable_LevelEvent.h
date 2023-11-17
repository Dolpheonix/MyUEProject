// Interactable - Level Event
// ���� �ܰ迡���� �̺�Ʈ�� ���ε���, ��ȣ�ۿ�� �̺�Ʈ�� �߻���ŵ�ϴ�.
#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Interactable_LevelEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractableLevelEvent);

UCLASS()
class QUICKSTART_API AInteractable_LevelEvent : public AInteractable
{
	GENERATED_BODY()
	
public:
	AInteractable_LevelEvent();
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif
public:
	// ���� �̺�Ʈ
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FInteractableLevelEvent LevelEvent;

	virtual void Interact() override;
	// �޽� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* MainMeshComponent;
	// �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* MainMesh;
};
