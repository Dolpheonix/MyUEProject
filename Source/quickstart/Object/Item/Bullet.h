// Item - Bullet
// 총알 오브젝트
// Projectile Component 구현, 발사체 메시를 추가해 블루프린트로 확장 가능하게 설계
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
	// World Static 오브젝트에 충돌 시 호출
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	// Enemy 등의 Dynamic 오브젝트에 충돌 시 호출
	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 충돌 처리 Component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;
	// 발사체 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;
	// 발사체 무브먼트
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	// 총알의 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BulletDamage = 30.0f;
	// 총알을 발사함
	void FireInDirection(const FVector& ShootDirection);
};
