#include "SniperController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "../../../Utils/Helpers.h"

ASniperController::ASniperController()
{
	PrimaryActorTick.bCanEverTick = true;

	BT = Helpers::C_LoadObjectFromPath<UBehaviorTree>(TEXT("/Game/ShootingGame/Character/Enemy/Sniper/Sniper_BT.Sniper_BT"));
	BB = Helpers::C_LoadObjectFromPath<UBlackboardData>(TEXT("/Game/ShootingGame/Character/Enemy/Sniper/Sniper_BB.Sniper_BB"));
}

void ASniperController::BeginPlay()
{
	Super::BeginPlay();
	OwnerSniper = Cast<ASniper>(GetCharacter());
}