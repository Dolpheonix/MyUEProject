// Volume - Force Field
// ���� ����
// ���η� ���� ���͵鿡 ���� ������ ���� ������ �̵���Ŵ
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/ArrowComponent.h"
#include "../WorldObject/PhysicsObject.h"
#include "../../Character/Character_Root.h"
#include "ForceField.generated.h"

// Force Field ���ο� ���� Character List ������
USTRUCT(Atomic, BlueprintType)
struct FEnterForceFieldForm_Character
{
	GENERATED_USTRUCT_BODY()

	ACharacter_Root* EnteredActor;
	FVector InitialVelocity;	// ���ο� ������ ������ Velocity

	FEnterForceFieldForm_Character() : EnteredActor(nullptr), InitialVelocity(FVector::ZeroVector) {}
	FEnterForceFieldForm_Character(ACharacter_Root* actor, FVector vel) : EnteredActor(actor), InitialVelocity(vel) {}
};

USTRUCT(Atomic, BlueprintType)
struct FEnterForceFieldForm_Object
{
	GENERATED_USTRUCT_BODY()

	APhysicsObject* EnteredActor;
	FVector InitialVelocity;	// ���ο� ������ ������ Velocity

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
	// ������ �۾� �� ������ ���� Ȯ�ο�
	UArrowComponent* Arrow;
	// ���ο� ���� Object ����Ʈ
	UPROPERTY(VisibleAnywhere, Category="Test")
	TArray<FEnterForceFieldForm_Object> ActorEntry;
	// ���ο� ���� Character ����Ʈ
	UPROPERTY()
	TArray<FEnterForceFieldForm_Character> CharacterEntry;
	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Force")
	FRotator ForceDirection = FRotator::ZeroRotator;
	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float ForceMagnitude = 1.f;
	// ������Ʈ �ʱ� �ӵ��� ���赵
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float DampingScale = 1.f;
	// ������ ���� ����
	FVector ForceVector = FVector::ZeroVector;
};