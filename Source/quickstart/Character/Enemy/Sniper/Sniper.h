// Enemy - Sniper
// 상대에게 총을 쏘는 Enemy Character

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

	// 정면으로 총을 발사
	UFUNCTION(BlueprintCallable)
	void Fire();

public:
	// 발사음 Audio Component
	UAudioComponent* FireAudioComponent;
	// 발사음 Sound Cue
	USoundCue* FireSound;
	// 총알 객체의 Class Object (ABullet 클래스로부터 확장)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassSelection")
	TSubclassOf<ABullet> ProjectileClass;
	// 총의 Mesh Comopnent
	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;
};