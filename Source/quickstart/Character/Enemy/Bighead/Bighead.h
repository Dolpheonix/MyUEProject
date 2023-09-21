// ABighead
// Enemy의 하위 클래스

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
	// 타겟에 공격 데미지 적용
	void Bump(AActor* hittee);
	
public:
	// 공격 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float BumpDamage = 10.0f;
	// 공격 효과 FX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Particle")
	UParticleSystemComponent* BumpAuraComponent;
};