#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Enemy/Enemy.h"
#include <Kismet/GameplayStatics.h>

void AMainCharacter::Fist()
{
	if (GetCurrentAction() != ECustomActionMode::ATTACK)
	{
		SetCurrentAction(ECustomActionMode::ATTACK);
		AttackPhase = 0;
	}
}

void AMainCharacter::Fire()
{
	if (ProjectileClass && GetCurrentAction() != ECustomActionMode::ATTACK)
	{
		SetCurrentAction(ECustomActionMode::ATTACK);
		AttackPhase = 0;
		if (FireAudio)
		{
			FireAudio->Play();
		}

		FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Rifle_Muzzle"));
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
	if (GetCurrentAction() != ECustomActionMode::ATTACK)
	{
		SetCurrentAction(ECustomActionMode::ATTACK);
		bContinueAttack = false;
		AttackPhase = 0;
		stamp = UGameplayStatics::GetTimeSeconds(this);
	}
	else
	{
		float now = UGameplayStatics::GetTimeSeconds(this);
		if (now - stamp <= 1.0f && now - stamp >= 0.5f && AttackPhase < 3)
		{
			bContinueAttack = true;
			stamp = stamp + 1.0f;
		}
	}
}

void AMainCharacter::CheckEndMovement()
{
	float CurrentPlayerTime;
	if (GetCurrentMovement() == ECustomMovementMode::JUMP)
	{
		CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(41);
		if (GetCurrentAction() == ECustomActionMode::ATTACK)
		{
			SetCurrentMovement(ECustomMovementMode::IDLE);
		}
		if (CurrentPlayerTime >= AnimationAssetPhases::Jump)
		{
			if (!bFalling)
			{
				if (MoveKeyPressed > 0)
				{
					SetCurrentMovement(ECustomMovementMode::WALK);
				}
				else SetCurrentMovement(ECustomMovementMode::IDLE);
			}
			else SetCurrentMovement(ECustomMovementMode::FALL);
		}
	}
	else if (GetCurrentMovement() == ECustomMovementMode::IDLE)
	{
		if (!bForced)
		{
			if (MoveKeyPressed > 0)
			{
				SetCurrentMovement(ECustomMovementMode::WALK);
			}
			if (bFalling)
			{
				SetCurrentMovement(ECustomMovementMode::FALL);
			}
		}
	}
	else if (GetCurrentMovement() == ECustomMovementMode::FALL)
	{
		if (bForced)
		{
			SetCurrentMovement(ECustomMovementMode::IDLE);
		}
		else if (!bFalling)
		{
			if (MoveKeyPressed > 0)
			{
				SetCurrentMovement(ECustomMovementMode::WALK);
			}
			else SetCurrentMovement(ECustomMovementMode::IDLE);
		}
	}
	else if (GetCurrentMovement() == ECustomMovementMode::WALK)
	{
		if (bForced) SetCurrentMovement(ECustomMovementMode::IDLE);
		else if (bFalling) SetCurrentMovement(ECustomMovementMode::FALL);
	}
}

void AMainCharacter::CheckEndAction()
{
	float CurrentPlayerTime;
	if (GetCurrentAction() == ECustomActionMode::ATTACK)
	{
		switch (WeaponCode)
		{
		case 0:
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(17);
			if (CurrentPlayerTime >= AnimationAssetPhases::Fist)
			{
				EndAction();
			}
			break;
		case 1:
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(15);
			if (CurrentPlayerTime >= AnimationAssetPhases::Fire)
			{
				EndAction();
			}
			break;
		case 2:
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(16);
			if (bAttackBlocked)
			{
				EndAction();
				bAttackBlocked = false;
			}
			else if (CurrentPlayerTime >= AnimationAssetPhases::Wield[AttackPhase])
			{
				if (bContinueAttack)
				{
					AttackPhase++;
					bContinueAttack = false;
				}
				else
				{
					EndAction();
				}
			}
			break;
		default:
			break;
		}
	}
	else if (GetCurrentAction() == ECustomActionMode::INTERACT)
	{
		CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(21);
		if (CurrentPlayerTime >= AnimationAssetPhases::Interact)
		{
			EndAction();
		}
	}
}

void AMainCharacter::EndAction()
{
	AttackPhase = -1;
	SetCurrentAction(ECustomActionMode::IDLE);
}

void AMainCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComponent->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		if (GetCurrentAction() != ECustomActionMode::ATTACK) // Sword is overlapped in idle/move state
		{
			auto Curr = Weapons[Weapon_Now].NameTag;
			Weapons[Weapon_Now].MeshComponent->SetSimulatePhysics(false);
			Weapons[Weapon_Now].MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Curr + "_Equip"));
		}
		else
		{
			auto Curr = Weapons[Weapon_Now].NameTag;
			Weapons[Weapon_Now].MeshComponent->SetSimulatePhysics(false);
			Weapons[Weapon_Now].MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Curr + "_Equip"));
			bAttackBlocked = true;
		}
	}
}

void AMainCharacter::OnOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Hittee = Cast<AEnemy>(OtherActor);
	if (Hittee && GetCurrentAction() == ECustomActionMode::ATTACK)
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