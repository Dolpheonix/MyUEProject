// Interactable - Lever
// 레버를 당겨 레벨 이벤트를 발동하는 오브젝트
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

	// 레버 이벤트
	UPROPERTY(BlueprintAssignable, Category="Event")
	FLeverTriggerEvent OnLeverTriggered;
	// 레버 발동 시 호출되는 함수
	UFUNCTION(BlueprintImplementableEvent)
	void Lever_On();
	// 레버의 케이스 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* LeverCase;
	// 레버의 지렛대 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* LeverBody;
	// 레버를 당겨진 상태인가?
	bool bLeverDown = false;
};
