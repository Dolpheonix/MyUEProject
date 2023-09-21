// Character - Enemy
// �÷��̾�� �����ϴ� Enemy�� �⺻ Ŭ����
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

// Enemy�� �� ���� ���
UENUM(BlueprintType)
enum class EEnemyDetectionMode : uint8
{
	PATROL,	// ���� ���� (���� �������� ����)
	CAUTION,	// ��� ���� (���� ó�� ����������, Ȯ������ ����)
	DETECTED,	// ���� ���� (���� Ȯ���� ����, �߰� �� ����)
	HURT,	// Hurt ���� (�������� ���� ����)
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

	// ó�� Caution / Detected ���¿� ���� �� ��Ҹ� ���
	UFUNCTION()
	void PlayDetectSound(bool isDoubt);
public:
	// AI ��Ʈ�ѷ�
	UPROPERTY(BlueprintReadWrite)
	AAIController* AIController;
	// HP ���� Ŭ���� ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassSelection")
	TSubclassOf<UHPBar> HPWidgetClass;
	// Caution �����϶� �����ִ� ����ǥ ǥ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	UBillboardComponent* QuestionMarkComponent;
	// Detected �����϶� �����ִ� ����ǥ ǥ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Billboard")
	UBillboardComponent* ExclamationMarkComponent;
	// HP ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Billboard")
	UWidgetComponent* HPWidget;
	// ��Ҹ� Audio Component
	UAudioComponent* DetectionAudioComponent;
	// Doubt Sound Cue (Caution ������ ��)
	USoundCue* DoubtingSound;
	// Detecting Sound Cue (Detected ������ ��)
	USoundCue* DetectingSound;
	// �ð� �ۼ����� �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float SightRadius = 1500.0f;
	// �ð� �ۼ����� ����� ��ġ�� �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float LoseSightRadius = 2000.0f;
	// �ð� �ۼ����� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	float PeripheralVisionAngleDegrees = 60.0f;
	// �ۼ����� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
	FAISenseAffiliationFilter DetectionByAffiliation = FAISenseAffiliationFilter();
	// Enemy�� Ư���� ��Ÿ���� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Label")
	TArray<FString> Labels;
};
