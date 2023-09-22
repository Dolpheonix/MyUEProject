// Volume - Portal
// �� ������ �ٸ� ������ �̵� ������ ��Ż
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
	// Pair ������Ƽ ���� ��, �̸� �ݴ��� ��Ż�� ����
	void UpdatePair(APortal* newpair);
	// �� ��Ż�� ����
	void Connect();
	// �� ��Ż�� ������ ����
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
	// ���� ����� ��Ż�� �ִ°�?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	bool bPairing = false;
	// ����� ��Ż
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal", meta = (EditCondition = "bPairing"))
	APortal* Pair;
	// �������� �� ��Ż ��� �� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* Destination;
	// ������ ����� ��Ż
	APortal* Oldpair;
};

