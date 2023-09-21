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

// Crowd Following Component 추가 (AI끼리 부딪히지 않음)
AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowing")))
{
	PrimaryActorTick.bCanEverTick = true;

	// Perception Component 생성 및 시각 퍼셉션 생성
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));
	SightSense = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));
	GetPerceptionComponent()->ConfigureSense(*SightSense);
	GetPerceptionComponent()->SetDominantSense(*SightSense->GetSenseImplementation());
	GetPerceptionComponent()->Activate();
	// 타겟 감지 시 호출
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdate);
	// 블랙보드 키 이름 설정
	PlayerKeyName = FName(TEXT("Player"));
	OriginLocationKeyName = FName(TEXT("OriginLocation"));
	HurtKeyName = FName(TEXT("Hurt"));
	// Enemy의 Team ID : 2
	SetGenericTeamId(FGenericTeamId(2));
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (BB && BT)
	{
		// Behavior Tree, Blackboard 실행
		UseBlackboard(BB, Blackboard);
		RunBehaviorTree(BT);
		// 블랙보드 변수 등록
		GetBlackboardComponent()->SetValueAsVector(OriginLocationKeyName, GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsEnum(FName("DetectionMode"), uint8(EEnemyDetectionMode::PATROL));
	}
	OwnerEnemy = Cast<AEnemy>(GetCharacter());
	if (OwnerEnemy)
	{
		// 시각 퍼셉션 Config를 적용
		UAISenseConfig_Sight* sight = Cast<UAISenseConfig_Sight>(GetPerceptionComponent()->GetSenseConfig(UAISense::GetSenseID<UAISense_Sight>()));
		sight->SightRadius = OwnerEnemy->SightRadius;
		sight->LoseSightRadius = OwnerEnemy->LoseSightRadius;
		sight->PeripheralVisionAngleDegrees = OwnerEnemy->PeripheralVisionAngleDegrees;
		sight->DetectionByAffiliation = OwnerEnemy->DetectionByAffiliation;
	}
	GetPerceptionComponent()->RequestStimuliListenerUpdate();	// 프로퍼티 update 적용

	MainPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector dir = (MainPlayer->GetActorLocation() - OwnerEnemy->GetActorLocation()).GetSafeNormal2D();
	FRotator rot = FRotationMatrix::MakeFromX(dir).Rotator();

	OwnerEnemy->HPWidget->SetWorldRotation(rot);	// HP 바를 플레이어 방향으로 회전
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
				
				// 물음표 표시
				OwnerEnemy->QuestionMarkComponent->SetVisibility(true);
				// Caution 목소리 재생
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
				GetWorld()->GetTimerManager().SetTimer(TargetHoldTimer, this, &AEnemyController::DetectedToPatrol, 0.2f, false, 5.0f);
			}
		}
	}
}

void AEnemyController::CautionToDetected()
{
	GetWorld()->GetTimerManager().ClearTimer(CautionToDetectedTimer);
	GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::DETECTED));
	// 느낌표 표시 및 물음표 숨김
	OwnerEnemy->ExclamationMarkComponent->SetVisibility(true);
	OwnerEnemy->QuestionMarkComponent->SetVisibility(false);
	// Detected 목소리 재생
	OwnerEnemy->PlayDetectSound(false);
}

void AEnemyController::DetectedToPatrol()
{
	GetBlackboardComponent()->SetValueAsObject(PlayerKeyName, nullptr);
	GetWorld()->GetTimerManager().ClearTimer(TargetHoldTimer);
	GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::PATROL));
	// 느낌표 숨김
	OwnerEnemy->ExclamationMarkComponent->SetVisibility(false);
}
