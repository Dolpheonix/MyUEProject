// Volume - Portal
// 한 곳에서 다른 곳으로 이동 가능한 포탈
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Portal.generated.h"

UCLASS()
class QUICKSTART_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal();

public:	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	void OnOverlapped_Teleport(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Pair 프로퍼티 변경 시, 이를 반대편 포탈에 적용
	void UpdatePair(APortal* newpair);
	// 두 포탈을 연결
	void Connect();
	// 두 포탈의 연결을 끊음
	void Disconnect();

public:
	// Collision Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* Volume;
	// Particle Component
	UPROPERTY(EdItAnywhere, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* FXComponent;
	// FX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	UParticleSystem* PortalFX;
	// 현재 연결된 포탈이 있는가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	bool bPairing = false;
	// 연결된 포탈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal", meta = (EditCondition = "bPairing"))
	APortal* Pair;
	// 도착지점 및 포탈 통과 시 나오는 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* Destination;
	// 이전에 연결된 포탈
	APortal* Oldpair;
};

