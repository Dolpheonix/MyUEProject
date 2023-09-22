// Volume - Beacon
// ���� ����Ʈ�� �������� �����Ǵ� ����
// �÷��̾�� ������ �ÿ� ����Ʈ�� �Ϸ�ó����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../../Utils/NPCUtil.h"
#include "Beacon.generated.h"

UCLASS()
class QUICKSTART_API ABeacon : public AActor
{
	GENERATED_BODY()
	
public:
	ABeacon();

	// �÷��̾�� ������ ��, ����Ʈ�� �Ϸ��Ű�� �ı���
	UFUNCTION()
	void OnOverlapped_Arrival(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// ���ܿ� ����Ʈ�� �����
	void RegisterQuest(FSingleQuest* quest);

public:
	// Collision Component
	UBoxComponent* Volume;
	// Indicator FX
	UParticleSystemComponent* FXComponent;
	// �Ϸ��ų ����Ʈ
	FSingleQuest* Ownerquest;
};
