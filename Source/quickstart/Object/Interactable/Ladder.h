// Interactable - Ladder
// �÷��̾ Ÿ�� �������� �� �ִ� ��ٸ�
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
	// ��ٸ� �޽� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LadderMesh;
	// ��ٸ��� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Slope;
	// ��ٸ��� �ʺ�
	float Width;

	FVector Left;
};
