#include "Bighead.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "../../Main/MainCharacter.h"
#include "../../../Utils/Helpers.h"

ABighead::ABighead()
{
	PrimaryActorTick.bCanEverTick = true;

	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Enemy/Bighead/Mesh/SK_Bighead.SK_Bighead")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector::ZeroVector, FRotator(0.0f, -90.0f, 0.0f));

	auto Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleHalfHeight(140.0f);
	Capsule->SetCapsuleRadius(140.0f);

	QuestionMarkComponent->SetRelativeLocation(FVector(115.0f, 0.0f, 115.0f));
	ExclamationMarkComponent->SetRelativeLocation(FVector(115.0f, 0.0f, 115.0f));
	HPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	BumpAuraComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Aura"));
	BumpAuraComponent->SetupAttachment(RootComponent);
	BumpAuraComponent->SetTemplate(Helpers::C_LoadObjectFromPath<UParticleSystem>(TEXT("/Game/ShootingGame/Particle/FX_VarietyPack/FX/P_ky_healAura.P_ky_healAura")));
}

void ABighead::BeginPlay()
{
	Super::BeginPlay();
	BumpAuraComponent->Deactivate();
}

void ABighead::Bump(AActor* hittee)
{
	if (hittee)
	{
		UGameplayStatics::ApplyDamage(hittee, BumpDamage, nullptr, this, NULL);
	}
}