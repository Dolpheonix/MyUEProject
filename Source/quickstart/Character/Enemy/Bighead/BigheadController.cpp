#include "BigheadController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "../../../Utils/Helpers.h"

ABigheadController::ABigheadController()
{
	PrimaryActorTick.bCanEverTick = true;

	BT = Helpers::C_LoadObjectFromPath<UBehaviorTree>(TEXT("/Game/ShootingGame/Character/Enemy/Bighead/Bighead_BT.Bighead_BT"));
	BB = Helpers::C_LoadObjectFromPath<UBlackboardData>(TEXT("/Game/ShootingGame/Character/Enemy/Bighead/Bighead_BB.Bighead_BB"));
}

void ABigheadController::BeginPlay()
{
	Super::BeginPlay();
	OwnerBighead = Cast<ABighead>(GetCharacter());
}