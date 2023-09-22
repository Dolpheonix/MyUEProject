// Item - Thorwable
// 플레이어가 던질 수 있는 오브젝트
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/Classes/Components/SphereComponent.h>
#include "GameFramework/ProjectileMovementComponent.h"
#include "Throwable.generated.h"

UCLASS()
class QUICKSTART_API AThrowable : public AActor
{
	GENERATED_BODY()
	
public:	
	AThrowable();

	UPROPERTY(BlueprintReadWrite)
	FString Tag;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION(BlueprintCallable)
	void Throw(const FVector& Direction);
};
