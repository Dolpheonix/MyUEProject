#pragma once

#include "CoreMinimal.h"
#include "../Enemy.h"
#include "../../../Object/Item/Bullet.h"
#include "Sniper.generated.h"

UCLASS()
class QUICKSTART_API ASniper : public AEnemy
{
	GENERATED_BODY()

public:
	ASniper();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void Fire();

public:
	UAudioComponent* FireAudioComponent;
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassSelection")
	TSubclassOf<ABullet> ProjectileClass;
	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;
};