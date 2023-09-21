// AI Controller - NPC Controller
// NPC 전용 AI 컨트롤러
// 비헤이비어 트리, 블랙보드 로드 및 키 등록
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPC.h"
#include "NPCController.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API ANPCController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANPCController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBehaviorTree* BT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBlackboardData* BB;

	ANPC* OwnerNPC;
};
