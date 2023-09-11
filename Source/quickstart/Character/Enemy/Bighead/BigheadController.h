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
	ABighead* OwnerBighead;
};