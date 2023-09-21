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
			FireAudio->Play();	// ���� �Ҹ� ���
		}

		FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Rifle_Muzzle"));	// �ѱ��� ��ġ
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, FRotator(0,0,0), SpawnParams); // �߻�ü ����
			if (Projectile)
			{
				FVector LaunchDirection = GetControlRotation().Vector();
				Projectile->FireInDirection(LaunchDirection); // �߻�ü �ӵ� ����
			}
		}
	}
}

void AMainCharacter::Wield()
{
	static float stamp = 0;
	if (GetCurrentAction() != ECustomActionMode::ATTACK)	// ó�� ������ ������ ��
	{
		SetCurrentAction(ECustomActionMode::ATTACK);
		bContinueAttack = false;
		AttackPhase = 0;
		stamp = UGameplayStatics::GetTimeSeconds(this);	// ���� ���� �ð�
	}
	else													// �ٽ� ���� ��ư�� ������ �� : �ٴ� ���� ��
	{
		float now = UGameplayStatics::GetTimeSeconds(this);	// ���� ��ư�� ���� �ð�
		if (now - stamp <= 1.0f && now - stamp >= 0.5f && AttackPhase < 3)	// 0.5�� ~ 1�� ���̿� ���� ���� ��ư�� ������ ���, ���� �ܰ��� ������ ����
		{
			bContinueAttack = true;
			stamp = stamp + 1.0f;	// ���� ������ ���� ������ ������� ���� ������ 1�� ��
		}
	}
}

void AMainCharacter::CheckEndMovement()
{
	float CurrentPlayerTime;
	if (GetCurrentMovement() == ECustomMovementMode::JUMP)	// JUMP ���¿����� Ʈ������ üũ
	{
		CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(41);	// Animation Blueprint����, ���� �ִϸ��̼� ��� �ð�
		if (GetCurrentAction() == ECustomActionMode::ATTACK)
		{
			SetCurrentMovement(ECustomMovementMode::IDLE);
		}
		if (CurrentPlayerTime >= AnimationAssetPhases::Jump)	// ���� �ð��� ����
		{
			if (!bFalling)	// ������ ���� ���� ����
			{
				if (MoveKeyPressed > 0)	// �̵�Ű�� ��������
				{
					SetCurrentMovement(ECustomMovementMode::WALK);
					FootstepAudioComponent->SetSound(WalkingSound);
					FootstepAudioComponent->SetPitchMultiplier(1.5f);
					FootstepAudioComponent->Play();
				}
				else SetCurrentMovement(ECustomMovementMode::IDLE);
			}
			else SetCurrentMovement(ECustomMovementMode::FALL);	// ���� ���� �ʾҴٸ�, Fall ���·� ��ȯ
		}
	}
	else if (GetCurrentMovement() == ECustomMovementMode::IDLE)	// IDLE ���¿����� Ʈ������ üũ
	{
		if (!bForced)	// �ܺ��� ������ ���� ���� ����
		{
			if (MoveKeyPressed > 0)	// �̵�Ű�� ��������
			{
				SetCurrentMovement(ECustomMovementMode::WALK);
				FootstepAudioComponent->SetSound(WalkingSound);
				FootstepAudioComponent->SetPitchMultiplier(1.5f);
				FootstepAudioComponent->Play();
			}
			if (bFalling)
			{
				SetCurrentMovement(ECustomMovementMode::FALL);
			}
		}
	}
	else if (GetCurrentMovement() == ECustomMovementMode::FALL)	// FALL ���¿����� Ʈ������ üũ
	{
		if (bForced)	// �ܺ��� ���� �޴� �����̸�, Idle �ִϸ��̼��� ���
		{
			SetCurrentMovement(ECustomMovementMode::IDLE);
		}
		else if (!bFalling)	// ���� ����
		{
			if (MoveKeyPressed > 0)
			{
				SetCurrentMovement(ECustomMovementMode::WALK);
				FootstepAudioComponent->SetSound(WalkingSound);
				FootstepAudioComponent->SetPitchMultiplier(1.5f);
				FootstepAudioComponent->Play();
			}
			else SetCurrentMovement(ECustomMovementMode::IDLE);
		}
	}
	else if (GetCurrentMovement() == ECustomMovementMode::WALK)
	{
		if (bForced || MoveKeyPressed == 0)	// �ܺ��� ���� �ްų�, �̵�Ű�� ��� ��
		{
			SetCurrentMovement(ECustomMovementMode::IDLE);
			FootstepAudioComponent->Stop();
		}
		else if (bFalling)
		{
			SetCurrentMovement(ECustomMovementMode::FALL);
			FootstepAudioComponent->Stop();
		}
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
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(17);	// ��ġ �ִϸ��̼��� ��� �ð�
			if (CurrentPlayerTime >= AnimationAssetPhases::Fist)
			{
				EndAction();
			}
			break;
		case 1:
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(15);	// ���� �ִϸ��̼��� ��� �ð�
			if (CurrentPlayerTime >= AnimationAssetPhases::Fire)
			{
				EndAction();
			}
			break;
		case 2:
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(16);	// �˼� �ִϸ��̼��� ��� �ð�
			if (bAttackBlocked)	// ������ ��������, ������ ����
			{
				EndAction();
				bAttackBlocked = false;
			}
			else if (CurrentPlayerTime >= AnimationAssetPhases::Wield[AttackPhase])	// ���� �ܰ迡���� ��� �ð��� �ѱ�
			{
				if (bContinueAttack)	// ���� ���� Ű�� ����
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
		CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(21);	// ��ȣ�ۿ� �ִϸ��̼��� ��� �ð�
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
		FItemForm* CurrWeapon = &Inventory[(uint8)ETypeTag::Weapon].ItemForms[Quickslots_Now[(uint8)ETypeTag::Weapon]];
		if (GetCurrentAction() != ECustomActionMode::ATTACK) // ���� ���°� �ƴ� �� ������ ��Ʈ ���� ==> �������� ��
		{
			auto Curr = CurrWeapon->ShortForm.NameTag;
			CurrWeapon->MeshComponent->SetSimulatePhysics(false);
			CurrWeapon->MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Curr + "_Equip"));
		}
		else	// ���� �߿� ��Ʈ ���� : World Static ������Ʈ�� �浹�� ����̹Ƿ�, ���� ��� ó��
		{
			auto Curr = CurrWeapon->ShortForm.NameTag;
			CurrWeapon->MeshComponent->SetSimulatePhysics(false);
			CurrWeapon->MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Curr + "_Equip"));
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
			UGameplayStatics::ApplyDamage(Hittee, 30.0f, GetController(), this, NULL);	// ������ �ܰ��� ������ �߰� �������� ����
		}
	}
}