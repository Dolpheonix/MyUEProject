// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "../../Utils/Helpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Kismet/GameplayStatics.h"

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

		GetBlackboardComponent()->SetValueAsVector(FName("OriginLocation"), GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject(FName("MainPlayer"), UGameplayStatics::GetPlayerPawn(this, 0));
	}
}

void ANPCController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetBlackboardComponent()->SetValueAsBool(FName("Interacted"), OwnerNPC->bInteracted);
}

