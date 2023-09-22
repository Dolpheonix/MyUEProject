// Volume - Force Field
// 역장 볼륨
// 내부로 들어온 액터들에 일정 방향의 힘을 적용해 이동시킴
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/ArrowComponent.h"
#include "../WorldObject/PhysicsObject.h"
#include "../../Character/Character_Root.h"
#include "ForceField.generated.h"

// Force Field 내부에 들어온 Character List 보관용
USTRUCT(Atomic, BlueprintType)
struct FEnterForceFieldForm_Character
{
	GENERATED_USTRUCT_BODY()

	ACharacter_Root* EnteredActor;
	FVector InitialVelocity;	// 내부에 들어오는 순간의 Velocity

	FEnterForceFieldForm_Character() : EnteredActor(nullptr), InitialVelocity(FVector::ZeroVector) {}
	FEnterForceFieldForm_Character(ACharacter_Root* actor, FVector vel) : EnteredActor(actor), InitialVelocity(vel) {}
};

USTRUCT(Atomic, BlueprintType)
struct FEnterForceFieldForm_Object
{
	GENERATED_USTRUCT_BODY()

	APhysicsObject* EnteredActor;
	FVector InitialVelocity;	// 내부에 들어오는 순간의 Velocity

	FEnterForceFieldForm_Object() : EnteredActor(nullptr), InitialVelocity(FVector::ZeroVector) {}
	FEnterForceFieldForm_Object(APhysicsObject* actor, FVector vel) : EnteredActor(actor), InitialVelocity(vel) {}
};

UCLASS(hidecategories = (CharacterMovement))
class QUICKSTART_API AForceField : public APhysicsVolume
{
	GENERATED_BODY()

public:
	AForceField();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void ActorEnteredVolume(class AActor* Other) override;

	virtual void ActorLeavingVolume(class AActor* Other) override;

public:
	// 에디터 작업 시 역장의 방향 확인용
	UArrowComponent* Arrow;
	// 내부에 들어온 Object 리스트
	UPROPERTY(VisibleAnywhere, Category="Test")
	TArray<FEnterForceFieldForm_Object> ActorEntry;
	// 내부에 들어온 Character 리스트
	UPROPERTY()
	TArray<FEnterForceFieldForm_Character> CharacterEntry;
	// 역장의 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force")
	FRotator ForceDirection = FRotator::ZeroRotator;
	// 역장의 세기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float ForceMagnitude = 1.f;
	// 오브젝트 초기 속도의 감쇠도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float DampingScale = 1.f;
	// 역장의 방향 벡터
	FVector ForceVector = FVector::ZeroVector;
};