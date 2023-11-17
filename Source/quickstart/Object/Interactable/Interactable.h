// Interactable - Interactable
// �÷��̾ ��ȣ�ۿ��� ������ ������Ʈ���� Base Class (NPC ����)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Character/Main/MainCharacter.h"
#include "../../Interface/InteractionInterface.h"
#include "Blueprint/UserWidget.h"
#include "Interactable.generated.h"

UCLASS(Abstract)
class QUICKSTART_API AInteractable : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AInteractable();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
protected:
	AMainCharacter* Player;
	// ��ȣ�ۿ� ���� ������ �߽���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
	FVector InteractPoint;
	// ��ȣ�ۿ� ���� ������ ���ؼ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FVector InteractForward;
	// ��ȣ�ۿ� ���� ������ �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
	float InteractRadius;
	// ��ȣ�ۿ� ���� ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	float InteractRange;

	// Interaction Forward�� �ð�ȭ
	UArrowComponent* InteractionForwardArrow;
	// ��ȣ�ۿ� ������ ������ ���� �ð�ȭ
	UArrowComponent* InteractionRangeArrow_R;
	// ��ȣ�ۿ� ������ ���� ���� �ð�ȭ
	UArrowComponent* InteractionRangeArrow_L;

	// ��ȣ�ۿ� ������ �����ΰ�?
	bool bInteractable = false;

	// ����ϴ� �����ΰ�? (�ѹ� ��ȣ�ۿ��ϰ�, �ٽ� �� �� ���� ������ �� ���)
	UPROPERTY(BlueprintReadWrite)
	bool bActive = true;
	// ��ٷ��� �ϴ� �����ΰ�?
	bool bWait = false;
};
