// AI Controller - Enemy Controller
// Enemy 액터의 컨트롤러

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
	// Player의 블랙보드 키
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName PlayerKeyName;
	// Enemy의 시작 위치 블랙보드 키
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName OriginLocationKeyName;
	// Hurt 블랙보드 키
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BlackBoardKey")
	FName HurtKeyName;

	// Behavior Tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBehaviorTree* BT;
	// Blackboard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBlackboardData* BB;
	// 컨트롤러의 오너
	AEnemy* OwnerEnemy;
	// 플레이어
	APawn* MainPlayer;
	// 시각 퍼셉션의 Config
	class UAISenseConfig_Sight* SightSense;

	FTimerHandle CautionToDetectedTimer;
	FTimerHandle TargetHoldTimer;

	// 액터의 퍼셉션 상태가 변경되었을 때(감지됨 / 감지 안됨) 호출
	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* SourceActor, FAIStimulus Stimulus);
	// 감지 모드를 Caution에서 Detected로 바꿈
	UFUNCTION()
	void CautionToDetected();
	// 감지 모드를 Detected에서 Patrol로 바꿈
	UFUNCTION()
	void DetectedToPatrol();
};
