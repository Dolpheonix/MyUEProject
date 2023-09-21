// Interface - Interaction Interface
// NPC, Interactable Item 등의 플레이어와 상호작용을 하는 오브젝트용 인터페이스
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

	// 상호작용 시에 호출
	virtual void Interact() {};
	// 상호작용이 끝날 때 호출
	virtual void UnInteract() {};
};
