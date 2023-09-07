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
	ASniperController();

protected:
	virtual void BeginPlay() override;

public:
	ASniper* OwnerSniper;
};