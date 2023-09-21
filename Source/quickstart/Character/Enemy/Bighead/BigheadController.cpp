#include "BigheadController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "../../../Utils/Helpers.h"

ABigheadController::ABigheadController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	// 비헤이비어 트리 & 블랙보드 로드
	BT = Helpers::C_LoadObjectFromPath<UBehaviorTree>(TEXT("/Game/ShootingGame/Character/Enemy/Bighead/Bighead_BT.Bighead_BT"));
	BB = Helpers::C_LoadObjectFromPath<UBlackboardData>(TEXT("/Game/ShootingGame/Character/Enemy/Bighead/Bighead_BB.Bighead_BB"));
}

void ABigheadController::BeginPlay()
{
	Super::BeginPlay();
	OwnerBighead = Cast<ABighead>(GetCharacter());
}