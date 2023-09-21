// Character - Enemy
// 플레이어와 대적하는 Enemy의 기본 클래스
#pragma once

#include "CoreMinimal.h"
#include "../Character_Root.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Components/WidgetComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "../../UI/HPBar.h"
#include "Enemy.generated.h"

// Enemy의 적 감지 모드
UENUM(BlueprintType)
enum class EEnemyDetectionMode : uint8
{
	PATROL,	// 순찰 상태 (적을 감지하지 못함)
	CAUTION,	// 경고 상태 (적을 처음 감지했으나, 확신하지 못함)
	DETECTED,	// 감지 상태 (적을 확실히 감지, 추격 및 공격)
	HURT,	// Hurt 상태 (데미지를 입은 상태)
	MAX,
};

UCLASS()
class QUICKSTART_API AEnemy : public ACharacter_Root
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnHurt() override;

	virtual void OnDead() override;

	// 처음 Caution / Detected 상태에 들어갔을 때 목소리 재생
	UFUNCTION()
	void PlayDetectSound(bool isDoubt);
public:
	// AI 컨트롤러
	UPROPERTY(BlueprintReadWrite)
	AAIController* AIController;
	// HP 바의 클래스 오브젝트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassSelection")
	TSubclassOf<UHPBar> HPWidgetClass;
	// Caution 상태일때 보여주는 물음표 표시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	UBillboardComponent* QuestionMarkComponent;
	// Detected 상태일때 보여주는 느낌표 표시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard")
	UBillboardComponent* ExclamationMarkComponent;
	// HP 바
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	UWidgetComponent* HPWidget;
	// 목소리 Audio Component
	UAudioComponent* DetectionAudioComponent;
	// Doubt Sound Cue (Caution 상태일 때)
	USoundCue* DoubtingSound;
	// Detecting Sound Cue (Detected 상태일 때)
	USoundCue* DetectingSound;
	// 시각 퍼셉션의 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float SightRadius = 1500.0f;
	// 시각 퍼셉션이 대상을 놓치는 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float LoseSightRadius = 2000.0f;
	// 시각 퍼셉션의 범위각
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float PeripheralVisionAngleDegrees = 60.0f;
	// 퍼셉션의 감지 필터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	FAISenseAffiliationFilter DetectionByAffiliation = FAISenseAffiliationFilter();
	// Enemy의 특성을 나타내는 라벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Label")
	TArray<FString> Labels;
};
