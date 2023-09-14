// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "../Character_Root.h"

AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowing")))
{
	PrimaryActorTick.bCanEverTick = true;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));
	SightSense = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));
	GetPerceptionComponent()->ConfigureSense(*SightSense);
	GetPerceptionComponent()->SetDominantSense(*SightSense->GetSenseImplementation());
	GetPerceptionComponent()->Activate();

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdate);

	PlayerKeyName = FName(TEXT("Player"));
	OriginLocationKeyName = FName(TEXT("OriginLocation"));
	HurtKeyName = FName(TEXT("Hurt"));

	SetGenericTeamId(FGenericTeamId(2));
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (BB && BT)
	{
		UseBlackboard(BB, Blackboard);
		RunBehaviorTree(BT);

		GetBlackboardComponent()->SetValueAsVector(OriginLocationKeyName, GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsEnum(FName("DetectionMode"), uint8(EEnemyDetectionMode::PATROL));
	}
	OwnerEnemy = Cast<AEnemy>(GetCharacter());
	if (OwnerEnemy)
	{
		UAISenseConfig_Sight* sight = Cast<UAISenseConfig_Sight>(GetPerceptionComponent()->GetSenseConfig(UAISense::GetSenseID<UAISense_Sight>()));
		sight->SightRadius = OwnerEnemy->SightRadius;
		sight->LoseSightRadius = OwnerEnemy->LoseSightRadius;
		sight->PeripheralVisionAngleDegrees = OwnerEnemy->PeripheralVisionAngleDegrees;
		sight->DetectionByAffiliation = OwnerEnemy->DetectionByAffiliation;
	}
	GetPerceptionComponent()->RequestStimuliListenerUpdate();

	MainPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector dir = (MainPlayer->GetActorLocation() - OwnerEnemy->GetActorLocation()).GetSafeNormal2D();
	FRotator rot = FRotationMatrix::MakeFromX(dir).Rotator();

	OwnerEnemy->HPWidget->SetWorldRotation(rot);
}

void AEnemyController::OnTargetPerceptionUpdate(AActor* SourceActor, FAIStimulus Stimulus)
{
	if (SourceActor == MainPlayer) // 플레이어의 감지 상태가 변함
	{
		EEnemyDetectionMode currDetection = EEnemyDetectionMode(GetBlackboardComponent()->GetValueAsEnum("DetectionMode")); // 현재 감지모드
		if (Stimulus.WasSuccessfullySensed()) // 감지 상태가 감지된 것으로 변함
		{
			if (currDetection == EEnemyDetectionMode::PATROL) // Patrol 상태
			{
				GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::CAUTION)); // Caution 모드로 변환
				GetWorld()->GetTimerManager().SetTimer(CautionToDetectedTimer, this, &AEnemyController::CautionToDetected, 0.5f, false, 2.0f); // 타이머 등록
				GetBlackboardComponent()->SetValueAsObject(PlayerKeyName, MainPlayer); // 플레이어를 블랙보드에 등록

				OwnerEnemy->QuestionMarkComponent->SetVisibility(true);
				OwnerEnemy->PlayDetectSound(true);
			}
			else if (currDetection == EEnemyDetectionMode::DETECTED) // Detected 상태인데 시야에 다시 들어옴
			{
				GetWorld()->GetTimerManager().ClearTimer(TargetHoldTimer);
			}
		}
		else // 감지 상태가 감지되지 않은 것으로 변함
		{
			if (currDetection == EEnemyDetectionMode::CAUTION)
			{
				GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::PATROL));
				GetWorld()->GetTimerManager().ClearTimer(CautionToDetectedTimer); // 타이머 삭제

				OwnerEnemy->QuestionMarkComponent->SetVisibility(false);
			}
			else if (currDetection == EEnemyDetectionMode::DETECTED) // Detected 상태인데 시야에서 벗어남
			{
				GetWorld()->GetTimerManager().SetTimer(TargetHoldTimer, this, &AEnemyController::IsTargetValid, 0.2f, false, 5.0f);
			}
		}
	}
}

void AEnemyController::CautionToDetected()
{
	GetWorld()->GetTimerManager().ClearTimer(CautionToDetectedTimer);
	GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::DETECTED));

	OwnerEnemy->ExclamationMarkComponent->SetVisibility(true);
	OwnerEnemy->QuestionMarkComponent->SetVisibility(false);
	OwnerEnemy->PlayDetectSound(false);
}

void AEnemyController::IsTargetValid()
{
	GetBlackboardComponent()->SetValueAsObject(PlayerKeyName, nullptr);
	GetWorld()->GetTimerManager().ClearTimer(TargetHoldTimer);
	GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::PATROL));

	OwnerEnemy->ExclamationMarkComponent->SetVisibility(false);
}
