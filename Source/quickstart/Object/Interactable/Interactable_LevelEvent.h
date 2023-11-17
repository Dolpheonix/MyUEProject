// Interactable - Level Event
// 레벨 단계에서의 이벤트를 바인드해, 상호작용시 이벤트를 발생시킵니다.
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
	// 레벨 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FInteractableLevelEvent LevelEvent;

	virtual void Interact() override;
	// 메시 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* MainMeshComponent;
	// 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* MainMesh;
};
