// Interface - Interaction Interface
// NPC, Interactable Item ���� �÷��̾�� ��ȣ�ۿ��� �ϴ� ������Ʈ�� �������̽�
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class QUICKSTART_API IInteractionInterface
{
	GENERATED_BODY()
public:

	// ��ȣ�ۿ� �ÿ� ȣ��
	virtual void Interact() {};
	// ��ȣ�ۿ��� ���� �� ȣ��
	virtual void UnInteract() {};
};
