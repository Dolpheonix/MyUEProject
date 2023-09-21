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

// Crowd Following Component �߰� (AI���� �ε����� ����)
AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowing")))
{
	PrimaryActorTick.bCanEverTick = true;

	// Perception Component ���� �� �ð� �ۼ��� ����
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));
	SightSense = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));
	GetPerceptionComponent()->ConfigureSense(*SightSense);
	GetPerceptionComponent()->SetDominantSense(*SightSense->GetSenseImplementation());
	GetPerceptionComponent()->Activate();
	// Ÿ�� ���� �� ȣ��
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdate);
	// ������ Ű �̸� ����
	PlayerKeyName = FName(TEXT("Player"));
	OriginLocationKeyName = FName(TEXT("OriginLocation"));
	HurtKeyName = FName(TEXT("Hurt"));
	// Enemy�� Team ID : 2
	SetGenericTeamId(FGenericTeamId(2));
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (BB && BT)
	{
		// Behavior Tree, Blackboard ����
		UseBlackboard(BB, Blackboard);
		RunBehaviorTree(BT);
		// ������ ���� ���
		GetBlackboardComponent()->SetValueAsVector(OriginLocationKeyName, GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsEnum(FName("DetectionMode"), uint8(EEnemyDetectionMode::PATROL));
	}
	OwnerEnemy = Cast<AEnemy>(GetCharacter());
	if (OwnerEnemy)
	{
		// �ð� �ۼ��� Config�� ����
		UAISenseConfig_Sight* sight = Cast<UAISenseConfig_Sight>(GetPerceptionComponent()->GetSenseConfig(UAISense::GetSenseID<UAISense_Sight>()));
		sight->SightRadius = OwnerEnemy->SightRadius;
		sight->LoseSightRadius = OwnerEnemy->LoseSightRadius;
		sight->PeripheralVisionAngleDegrees = OwnerEnemy->PeripheralVisionAngleDegrees;
		sight->DetectionByAffiliation = OwnerEnemy->DetectionByAffiliation;
	}
	GetPerceptionComponent()->RequestStimuliListenerUpdate();	// ������Ƽ update ����

	MainPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector dir = (MainPlayer->GetActorLocation() - OwnerEnemy->GetActorLocation()).GetSafeNormal2D();
	FRotator rot = FRotationMatrix::MakeFromX(dir).Rotator();

	OwnerEnemy->HPWidget->SetWorldRotation(rot);	// HP �ٸ� �÷��̾� �������� ȸ��
}

void AEnemyController::OnTargetPerceptionUpdate(AActor* SourceActor, FAIStimulus Stimulus)
{
	if (SourceActor == MainPlayer) // �÷��̾��� ���� ���°� ����
	{
		EEnemyDetectionMode currDetection = EEnemyDetectionMode(GetBlackboardComponent()->GetValueAsEnum("DetectionMode")); // ���� �������
		if (Stimulus.WasSuccessfullySensed()) // ���� ���°� ������ ������ ����
		{
			if (currDetection == EEnemyDetectionMode::PATROL) // Patrol ����
			{
				GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::CAUTION)); // Caution ���� ��ȯ
				GetWorld()->GetTimerManager().SetTimer(CautionToDetectedTimer, this, &AEnemyController::CautionToDetected, 0.5f, false, 2.0f); // Ÿ�̸� ���
				GetBlackboardComponent()->SetValueAsObject(PlayerKeyName, MainPlayer); // �÷��̾ �����忡 ���
				
				// ����ǥ ǥ��
				OwnerEnemy->QuestionMarkComponent->SetVisibility(true);
				// Caution ��Ҹ� ���
				OwnerEnemy->PlayDetectSound(true);
			}
			else if (currDetection == EEnemyDetectionMode::DETECTED) // Detected �����ε� �þ߿� �ٽ� ����
			{
				GetWorld()->GetTimerManager().ClearTimer(TargetHoldTimer);
			}
		}
		else // ���� ���°� �������� ���� ������ ����
		{
			if (currDetection == EEnemyDetectionMode::CAUTION)
			{
				GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::PATROL));
				GetWorld()->GetTimerManager().ClearTimer(CautionToDetectedTimer); // Ÿ�̸� ����

				OwnerEnemy->QuestionMarkComponent->SetVisibility(false);
			}
			else if (currDetection == EEnemyDetectionMode::DETECTED) // Detected �����ε� �þ߿��� ���
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
	// ����ǥ ǥ�� �� ����ǥ ����
	OwnerEnemy->ExclamationMarkComponent->SetVisibility(true);
	OwnerEnemy->QuestionMarkComponent->SetVisibility(false);
	// Detected ��Ҹ� ���
	OwnerEnemy->PlayDetectSound(false);
}

void AEnemyController::DetectedToPatrol()
{
	GetBlackboardComponent()->SetValueAsObject(PlayerKeyName, nullptr);
	GetWorld()->GetTimerManager().ClearTimer(TargetHoldTimer);
	GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::PATROL));
	// ����ǥ ����
	OwnerEnemy->ExclamationMarkComponent->SetVisibility(false);
}
