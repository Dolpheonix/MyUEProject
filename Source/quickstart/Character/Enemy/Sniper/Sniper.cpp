#include "Sniper.h"
#include "SniperController.h"
#include "../../../Utils/Helpers.h"

ASniper::ASniper()
{
	PrimaryActorTick.bCanEverTick = true;

	// �޽� ������Ʈ ���� �� ��ġ ����
	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	// �߻��� �ε� �� ����� ������Ʈ�� ���
	FireSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Explosion_01_Cue.Explosion_01_Cue"));
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Audio"));
	FireAudioComponent->SetupAttachment(RootComponent);
	FireAudioComponent->SetSound(FireSound);
	// �� �޽� �ε� �� ��ġ ����
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rifle"));
	WeaponMesh->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName("Rifle")));
	WeaponMesh->SetCollisionProfileName("Weapon");
	WeaponMesh->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName("Rifle_Equip"));
	// AI ��Ʈ�ѷ� Ŭ���� ����
	AIControllerClass = ASniperController::StaticClass();
	// �� �߰�
	Labels.Add("Sniper");
}

void ASniper::Fire()
{
	// �ѱ� ��ġ
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Rifle_Muzzle"));
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, FRotator(0, 0, 0), SpawnParams);	// �߻�ü ���� ����
		if (Projectile)
		{
			FVector LaunchDirection = GetControlRotation().Vector();	// �߻� ���� ����
			Projectile->FireInDirection(LaunchDirection);

			FireAudioComponent->Play();	// �߻��� ���
		}
	}
}
