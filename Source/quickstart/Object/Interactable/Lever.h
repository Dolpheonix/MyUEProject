// Interactable - Lever
// ������ ��� ���� �̺�Ʈ�� �ߵ��ϴ� ������Ʈ
#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Lever.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeverTriggerEvent);

UCLASS()
class QUICKSTART_API ALever : public AInteractable
{
	GENERATED_BODY()
	
public:
	ALever();

protected:
	virtual void BeginPlay() override;
	virtual void Interact() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ���� �̺�Ʈ
	UPROPERTY(BlueprintAssignable, Category="Event")
	FLeverTriggerEvent OnLeverTriggered;
	// ���� �ߵ� �� ȣ��Ǵ� �Լ�
	UFUNCTION(BlueprintImplementableEvent)
	void Lever_On();
	// ������ ���̽� �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* LeverCase;
	// ������ ������ �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* LeverBody;
	// ������ ����� �����ΰ�?
	bool bLeverDown = false;
};
