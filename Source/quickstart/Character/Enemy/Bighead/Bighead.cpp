#include "Bighead.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "../../Main/MainCharacter.h"
#include "BigheadController.h"
#include "../../../Utils/Helpers.h"

ABighead::ABighead()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시 로드
	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Enemy/Bighead/Mesh/SK_Bighead.SK_Bighead")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f), FVector(100.0f, 100.0f, 100.0f));
	// Collision Capsule 설정
	auto Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleHalfHeight(140.0f);
	Capsule->SetCapsuleRadius(100.0f);
	// 기타 컴포넌트 위치 설정
	QuestionMarkComponent->SetRelativeLocation(FVector(115.0f, 0.0f, 115.0f));
	ExclamationMarkComponent->SetRelativeLocation(FVector(115.0f, 0.0f, 115.0f));
	HPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	// 파티클 로드
	BumpAuraComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Aura"));
	BumpAuraComponent->SetupAttachment(RootComponent);
	BumpAuraComponent->SetTemplate(Helpers::C_LoadObjectFromPath<UParticleSystem>(TEXT("/Game/ShootingGame/Particle/FX_VarietyPack/FX/P_ky_explosion.P_ky_explosion")));
	// AI 컨트롤러 설정
	AIControllerClass = ABigheadController::StaticClass();
	// 라벨 추가
	Labels.Add("Bighead");
}

void ABighead::BeginPlay()
{
	Super::BeginPlay();
	BumpAuraComponent->Deactivate();	// 파티클 비활성화 (공격 시에만 활성화)
}

void ABighead::Bump(AActor* hittee)
{
	if (hittee)
	{
		UGameplayStatics::ApplyDamage(hittee, BumpDamage, nullptr, this, NULL); // 타켓에 데미지 적용
	}
}