#include "Sniper.h"
#include "SniperController.h"
#include "../../../Utils/Helpers.h"

ASniper::ASniper()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시 컴포넌트 생성 및 위치 조정
	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	// 발사음 로드 및 오디오 컴포넌트에 등록
	FireSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Explosion_01_Cue.Explosion_01_Cue"));
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Audio"));
	FireAudioComponent->SetupAttachment(RootComponent);
	FireAudioComponent->SetSound(FireSound);
	// 총 메시 로드 및 위치 조정
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rifle"));
	WeaponMesh->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName("Rifle")));
	WeaponMesh->SetCollisionProfileName("Weapon");
	WeaponMesh->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName("Rifle_Equip"));
	// AI 컨트롤러 클래스 설정
	AIControllerClass = ASniperController::StaticClass();
	// 라벨 추가
	Labels.Add("Sniper");
}

void ASniper::Fire()
{
	// 총구 위치
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Rifle_Muzzle"));
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, FRotator(0, 0, 0), SpawnParams);	// 발사체 액터 스폰
		if (Projectile)
		{
			FVector LaunchDirection = GetControlRotation().Vector();	// 발사 방향 설정
			Projectile->FireInDirection(LaunchDirection);

			FireAudioComponent->Play();	// 발사음 재생
		}
	}
}
