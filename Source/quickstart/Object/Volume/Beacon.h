// Volume - Beacon
// 도착 퀘스트의 도착지에 생성되는 볼륨
// 플레이어와 오버랩 시에 퀘스트를 완료처리함
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

	// 플레이어와 오버랩 시, 퀘스트를 완료시키고 파괴됨
	UFUNCTION()
	void OnOverlapped_Arrival(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 비콘에 퀘스트를 등록함
	void RegisterQuest(FSingleQuest* quest);

public:
	// Collision Component
	UBoxComponent* Volume;
	// Indicator FX
	UParticleSystemComponent* FXComponent;
	// 완료시킬 퀘스트
	FSingleQuest* Ownerquest;
};
