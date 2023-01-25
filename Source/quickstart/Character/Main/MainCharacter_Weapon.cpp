#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Enemy/Enemy.h"
#include <Kismet/GameplayStatics.h>

void AMainCharacter::Fist()
{
	static float stamp = 0.0f;
	if (!bAttacking)
	{
		bAttacking = true;
		bContinueAttack = false;
		stamp = UGameplayStatics::GetTimeSeconds(this);
	}
	else
	{
		float now = UGameplayStatics::GetTimeSeconds(this);
		if (now - stamp <= 1.0f && now - stamp >= 0.5f)
		{
			bContinueAttack = true;
			stamp = stamp + 1.0f;
		}
	}
}

void AMainCharacter::Fire()
{
	if (ProjectileClass && !bAttacking)
	{
		bAttacking = true;
		if (FireAudio)
		{
			FireAudio->Play();
		}

		FVector MuzzleLocation = GetActorLocation() + FTransform(GetControlRotation()).TransformVector(Muzzle);
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator(); // instigator : spawn을 trigger한 주체
			ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, FRotator(0,0,0), SpawnParams); // world에 actor를 스폰
			if (Projectile)
			{
				FVector LaunchDirection = GetControlRotation().Vector();
				Projectile->FireInDirection(LaunchDirection); // 발사체 velocity 결정
			}
		}
	}
}

void AMainCharacter::Wield()
{
	static float stamp = 0;
	if (!bAttacking)
	{
		bAttacking = true;
		bContinueAttack = false;
		AttackPhase = 0;
		stamp = UGameplayStatics::GetTimeSeconds(this);
	}
	else
	{
		float now = UGameplayStatics::GetTimeSeconds(this);
		if (now - stamp <= 1.0f && now - stamp >= 0.5f)
		{
			bContinueAttack = true;
			AttackPhase++;
			stamp = stamp + 1.0f;
		}
	}
}

void AMainCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AMainCharacter::OnOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Hittee = Cast<AEnemy>(OtherActor);
	if (Hittee && bAttacking)
	{
		if (AttackPhase >= 0 && AttackPhase < 3)
		{
			UGameplayStatics::ApplyDamage(Hittee, 15.0f, GetController(), this, NULL);
		}
		else
		{
			UGameplayStatics::ApplyDamage(Hittee, 30.0f, GetController(), this, NULL);
		}
	}
}