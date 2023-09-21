#include "NPCController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "../../Utils/Helpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/GameplayStatics.h"

// Crowd Following Component : AI끼리 부딪히는 경우 방지
ANPCController::ANPCController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	BT = Helpers::C_LoadObjectFromPath<UBehaviorTree>(TEXT("/Game/ShootingGame/Character/NPC/NPC_BT.NPC_BT"));
	BB = Helpers::C_LoadObjectFromPath<UBlackboardData>(TEXT("/Game/ShootingGame/Character/NPC/NPC_BB.NPC_BB"));
}

void ANPCController::BeginPlay()
{
	Super::BeginPlay();

	OwnerNPC = Cast<ANPC>(GetCharacter());

	if (BB && BT && OwnerNPC)
	{
		UseBlackboard(BB, Blackboard);
		RunBehaviorTree(BT);

		GetBlackboardComponent()->SetValueAsVector(FName("OriginLocation"), GetPawn()->GetActorLocation());	// NPC의 시작 위치
		GetBlackboardComponent()->SetValueAsObject(FName("MainPlayer"), UGameplayStatics::GetPlayerPawn(this, 0));	// 플레이어
	}
}

void ANPCController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetBlackboardComponent()->SetValueAsBool(FName("Interacted"), OwnerNPC->bInteracted);	// 상호작용 여부를 블랙보드로 넘김
}

