// Interactable - Ladder
// 플레이어가 타고 오르내릴 수 있는 사다리
#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Ladder.generated.h"

UCLASS()
class QUICKSTART_API ALadder : public AInteractable
{
	GENERATED_BODY()

public:
	ALadder();

protected:
	virtual void BeginPlay() override;
	virtual void Interact() override;

public:
	virtual void Tick(float DeltaTime) override;
	// 사다리 메시 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LadderMesh;
	// 사다리의 경사
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Slope;
	// 사다리의 너비
	float Width;

	FVector Left;
};
