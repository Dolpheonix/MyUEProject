#pragma once

#include "CoreMinimal.h"
#include "../Enemy.h"
#include "Bighead.generated.h"

UCLASS()
class QUICKSTART_API ABighead : public AEnemy
{
	GENERATED_BODY()

public:
	ABighead();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
	float BumpDamage = 10.0f;
};