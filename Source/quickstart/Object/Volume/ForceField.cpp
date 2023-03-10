// Fill out your copyright notice in the Description page of Project Settings.


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
		Arrow->SetRelativeRotation(ForceDirection);
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

	auto player = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (isActivate)
	{	
		player->GetCharacterMovement()->AddInputVector(ForceVector * ForceMagnitude);
	}

	for (int i = 0; i < ActorEntry.Num(); i++)
	{
		ActorEntry[i].EnteredActor->GetPhysicsComponent()->AddImpulse(ForceVector * ForceMagnitude * 1000.f);
	}
}

void AForceField::ActorEnteredVolume(AActor* other)
{
	APhysicsVolume::ActorEnteredVolume(other);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("ENTER??"));
	auto player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if ((AActor*)player == other)
	{
		isActivate = true;
		player->GetCharacterMovement()->GravityScale = 0.f;
		player->GetCharacterMovement()->Velocity = FVector::ZeroVector;
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
			ActorEntry.Add(FEnterForceFieldForm(object, object->GetVelocity()));
		}
	}
}

void AForceField::ActorLeavingVolume(AActor* other)
{
	APhysicsVolume::ActorLeavingVolume(other);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("LEAVE??"));

	auto player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if ((AActor*)player == other)
	{
		player->GetCharacterMovement()->GravityScale = 1.f;
		isActivate = false;
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
