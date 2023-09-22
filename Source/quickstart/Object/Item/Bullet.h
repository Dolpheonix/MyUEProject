// Item - Bullet
// �Ѿ� ������Ʈ
// Projectile Component ����, �߻�ü �޽ø� �߰��� �������Ʈ�� Ȯ�� �����ϰ� ����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/Classes/Components/SphereComponent.h>
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class QUICKSTART_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

public:	
	// World Static ������Ʈ�� �浹 �� ȣ��
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	// Enemy ���� Dynamic ������Ʈ�� �浹 �� ȣ��
	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// �浹 ó�� Component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;
	// �߻�ü �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;
	// �߻�ü �����Ʈ
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	// �Ѿ��� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BulletDamage = 30.0f;
	// �Ѿ��� �߻���
	void FireInDirection(const FVector& ShootDirection);
};
