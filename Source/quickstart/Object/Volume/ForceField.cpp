#include "ForceField.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Utils/Helpers.h"
#include "Components/BrushComponent.h"

AForceField::AForceField()
{
	PrimaryActorTick.bCanEverTick = true;

	GetBrushComponent()->SetCollisionProfileName("Volume");

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Helpers::SetComponent(&Arrow, GetBrushComponent(), FVector::ZeroVector, FRotator::ZeroRotator);
}

#if WITH_EDITOR
void AForceField::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	auto propName = PropertyChangedEvent.MemberProperty->GetFName();
	if (propName == "ForceDirection")
	{
		Arrow->SetRelativeRotation(ForceDirection);	// Force Direction 프로퍼티에 맞춰서 화살표 방향 변경
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AForceField::BeginPlay()
{
	Super::BeginPlay();
	ForceVector = Arrow->GetComponentRotation().Vector();
}

void AForceField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 내부에 들어온 Object/Character들에게 힘 적용
	for (int i = 0; i < CharacterEntry.Num(); i++)
	{
		CharacterEntry[i].EnteredActor->GetCharacterMovement()->AddInputVector(ForceVector * ForceMagnitude);
	}

	for (int i = 0; i < ActorEntry.Num(); i++)
	{
		ActorEntry[i].EnteredActor->GetPhysicsComponent()->AddImpulse(ForceVector * ForceMagnitude * 1000.f);
	}
}

void AForceField::ActorEnteredVolume(AActor* other)
{
	APhysicsVolume::ActorEnteredVolume(other);

	// 리스트에 Character/Object 추가
	auto character = Cast<ACharacter_Root>(other);
	if (character)
	{
		character->GetCharacterMovement()->GravityScale = 0.f;
		character->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		character->bForced = true;
		CharacterEntry.Add(FEnterForceFieldForm_Character(character, character->GetVelocity()));
	}
	else
	{
		auto object = Cast<APhysicsObject>(other);
		if (object)
		{
			object->GetPhysicsComponent()->SetEnableGravity(false);
			FVector linear = object->GetPhysicsComponent()->GetPhysicsLinearVelocity() * DampingScale;
			FVector angular = object->GetPhysicsComponent()->GetPhysicsAngularVelocityInDegrees() * DampingScale;
			object->GetPhysicsComponent()->SetPhysicsLinearVelocity(linear);
			object->GetPhysicsComponent()->SetPhysicsAngularVelocityInDegrees(angular);
			ActorEntry.Add(FEnterForceFieldForm_Object(object, object->GetVelocity()));
		}
	}
}

void AForceField::ActorLeavingVolume(AActor* other)
{
	APhysicsVolume::ActorLeavingVolume(other);

	// 리스트에서 Character/Object 제거
	auto character = Cast<ACharacter_Root>(other);
	if (character)
	{
		int index;
		for (index = 0; index < CharacterEntry.Num(); index++)
		{
			if (CharacterEntry[index].EnteredActor == character)
			{
				CharacterEntry[index].EnteredActor->bForced = false;
				CharacterEntry[index].EnteredActor->GetCharacterMovement()->GravityScale = 1.f;
				break;
			}
		}
		CharacterEntry.RemoveAt(index);
	}
	else
	{
		auto object = Cast<APhysicsObject>(other);
		if (object)
		{
			int index;
			for (index = 0; index < ActorEntry.Num(); index++)
			{
				if (ActorEntry[index].EnteredActor == object)
				{
					ActorEntry[index].EnteredActor->GetPhysicsComponent()->SetEnableGravity(true);
					break;
				}
			}
			ActorEntry.RemoveAt(index);
		}
	}
}
