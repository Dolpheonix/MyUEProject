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
#include "../Character_Root.h"

AEnemyController::AEnemyController()
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
	if (SourceActor == MainPlayer)
	{
		EEnemyDetectionMode currDetection = EEnemyDetectionMode(GetBlackboardComponent()->GetValueAsEnum("DetectionMode"));
		if (Stimulus.WasSuccessfullySensed())
		{
			if (currDetection == EEnemyDetectionMode::PATROL)
			{
				GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::CAUTION));
				GetWorld()->GetTimerManager().SetTimer(CautionToDetectedTimer, this, &AEnemyController::CautionToDetected, 0.5f, false, 2.0f);
				GetBlackboardComponent()->SetValueAsObject(PlayerKeyName, MainPlayer);

				OwnerEnemy->QuestionMarkComponent->SetVisibility(true);
				OwnerEnemy->PlayDetectSound(true);
			}
			else if (currDetection == EEnemyDetectionMode::DETECTED)
			{
				GetWorld()->GetTimerManager().ClearTimer(TargetHoldTimer);
			}
		}
		else
		{
			if (currDetection == EEnemyDetectionMode::CAUTION)
			{
				GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::PATROL));
				GetWorld()->GetTimerManager().ClearTimer(CautionToDetectedTimer);

				OwnerEnemy->QuestionMarkComponent->SetVisibility(false);
			}
			else if (currDetection == EEnemyDetectionMode::DETECTED)
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
