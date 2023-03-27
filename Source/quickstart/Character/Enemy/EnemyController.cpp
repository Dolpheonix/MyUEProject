// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "../Character_Root.h"
#include "../../Utils/Helpers.h"

AEnemyController::AEnemyController()
{
	PlayerKey = FName(TEXT("Player"));
	OriginLocationKey = FName(TEXT("OriginLocation"));
	HurtKey = FName(TEXT("Hurt"));

	BT = Helpers::C_LoadObjectFromPath<UBehaviorTree>(TEXT("/Game/ShootingGame/Character/Enemy/Enemy_BT.Enemy_BT"));
	BB = Helpers::C_LoadObjectFromPath<UBlackboardData>(TEXT("/Game/ShootingGame/Character/Enemy/Enemy_BB.Enemy_BB"));
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (BB && BT)
	{
		UseBlackboard(BB, Blackboard);
		RunBehaviorTree(BT);

		GetBlackboardComponent()->SetValueAsObject(PlayerKey, UGameplayStatics::GetPlayerPawn(this, 0));
		GetBlackboardComponent()->SetValueAsVector(OriginLocationKey, GetPawn()->GetActorLocation());
	}
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto EnemySelf = Cast<ACharacter_Root>(GetPawn());
	GetBlackboardComponent()->SetValueAsBool(HurtKey, EnemySelf->bHurt);
}

