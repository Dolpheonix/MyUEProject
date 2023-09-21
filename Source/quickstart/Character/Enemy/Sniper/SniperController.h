// Enemy Controller - Sniper Controller
// Sniper 전용 AI Controller

#pragma once

#include "CoreMinimal.h"
#include "../EnemyController.h"
#include "Sniper.h"
#include "SniperController.generated.h"

UCLASS()
class QUICKSTART_API ASniperController : public AEnemyController
{
	GENERATED_BODY()

public:
	ASniperController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	// 컨트롤러 오너
	ASniper* OwnerSniper;
};