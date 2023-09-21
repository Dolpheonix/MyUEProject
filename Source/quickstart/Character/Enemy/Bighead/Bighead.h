// ABighead
// Enemy�� ���� Ŭ����

#pragma once

#include "CoreMinimal.h"
#include "../Enemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Bighead.generated.h"

UCLASS()
class QUICKSTART_API ABighead : public AEnemy
{
	GENERATED_BODY()

public:
	ABighead();
protected:
	virtual void BeginPlay() override;
public:
	// Ÿ�ٿ� ���� ������ ����
	void Bump(AActor* hittee);
	
public:
	// ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float BumpDamage = 10.0f;
	// ���� ȿ�� FX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Particle")
	UParticleSystemComponent* BumpAuraComponent;
};