#include "Bighead.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "../../Character_Root.h"
#include "../../../Utils/Helpers.h"

ABighead::ABighead()
{
	PrimaryActorTick.bCanEverTick = true;

	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Enemy/Bighead/Mesh/SK_Bighead.SK_Bighead")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator);

	auto Capsule = GetCapsuleComponent();
	Capsule->OnComponentHit.AddDynamic(this, &ABighead::OnHit);
}

void ABighead::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	auto Hittee = Cast<ACharacter_Root>(OtherActor);
	if (Hittee)
	{
		UGameplayStatics::ApplyDamage(Hittee, BumpDamage, nullptr, this, NULL);
	}
}