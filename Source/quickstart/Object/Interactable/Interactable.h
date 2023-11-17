// Interactable - Interactable
// 플레이어가 상호작용이 가능한 오브젝트들의 Base Class (NPC 제외)
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
	// 상호작용 가능 범위의 중심점
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
	FVector InteractPoint;
	// 상호작용 가능 범위의 기준선
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FVector InteractForward;
	// 상호작용 가능 범위의 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
	float InteractRadius;
	// 상호작용 가능 범위의 범위각
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	float InteractRange;

	// Interaction Forward의 시각화
	UArrowComponent* InteractionForwardArrow;
	// 상호작용 범위의 오른쪽 끝을 시각화
	UArrowComponent* InteractionRangeArrow_R;
	// 상호작용 범위의 왼쪽 끝을 시각화
	UArrowComponent* InteractionRangeArrow_L;

	// 상호작용 가능한 상태인가?
	bool bInteractable = false;

	// 기능하는 상태인가? (한번 상호작용하고, 다시 쓸 수 없는 상태일 때 사용)
	UPROPERTY(BlueprintReadWrite)
	bool bActive = true;
	// 기다려야 하는 상태인가?
	bool bWait = false;
};
