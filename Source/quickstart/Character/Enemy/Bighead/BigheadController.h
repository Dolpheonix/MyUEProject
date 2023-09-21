// ABigheadController
// ABighead 전용 AI 컨트롤러

#pragma once

#include "CoreMinimal.h"
#include "../EnemyController.h"
#include "Bighead.h"
#include "BigheadController.generated.h"

UCLASS()
class QUICKSTART_API ABigheadController : public AEnemyController
{
	GENERATED_BODY()
public:
	ABigheadController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	// 컨트롤러의 오너 액터
	ABighead* OwnerBighead;
};