// AI Controller - Enemy Controller
// Enemy ������ ��Ʈ�ѷ�

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enemy.h"
#include "EnemyController.generated.h"

UCLASS()
class QUICKSTART_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// Player�� ������ Ű
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName PlayerKeyName;
	// Enemy�� ���� ��ġ ������ Ű
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName OriginLocationKeyName;
	// Hurt ������ Ű
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName HurtKeyName;

	// Behavior Tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBehaviorTree* BT;
	// Blackboard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBlackboardData* BB;
	// ��Ʈ�ѷ��� ����
	AEnemy* OwnerEnemy;
	// �÷��̾�
	APawn* MainPlayer;
	// �ð� �ۼ����� Config
	class UAISenseConfig_Sight* SightSense;

	FTimerHandle CautionToDetectedTimer;
	FTimerHandle TargetHoldTimer;

	// ������ �ۼ��� ���°� ����Ǿ��� ��(������ / ���� �ȵ�) ȣ��
	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* SourceActor, FAIStimulus Stimulus);
	// ���� ��带 Caution���� Detected�� �ٲ�
	UFUNCTION()
	void CautionToDetected();
	// ���� ��带 Detected���� Patrol�� �ٲ�
	UFUNCTION()
	void DetectedToPatrol();
};
