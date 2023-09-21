// Enemy - Sniper
// ��뿡�� ���� ��� Enemy Character

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

	// �������� ���� �߻�
	UFUNCTION(BlueprintCallable)
	void Fire();

public:
	// �߻��� Audio Component
	UAudioComponent* FireAudioComponent;
	// �߻��� Sound Cue
	USoundCue* FireSound;
	// �Ѿ� ��ü�� Class Object (ABullet Ŭ�����κ��� Ȯ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClassSelection")
	TSubclassOf<ABullet> ProjectileClass;
	// ���� Mesh Comopnent
	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;
};