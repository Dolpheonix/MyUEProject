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
			FireAudio->Play();	// 발포 소리 재생
		}

		FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Rifle_Muzzle"));	// 총구의 위치
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, FRotator(0,0,0), SpawnParams); // 발사체 스폰
			if (Projectile)
			{
				FVector LaunchDirection = GetControlRotation().Vector();
				Projectile->FireInDirection(LaunchDirection); // 발사체 속도 결정
			}
		}
	}
}

void AMainCharacter::Wield()
{
	static float stamp = 0;
	if (GetCurrentAction() != ECustomActionMode::ATTACK)	// 처음 공격을 시작할 때
	{
		SetCurrentAction(ECustomActionMode::ATTACK);
		bContinueAttack = false;
		AttackPhase = 0;
		stamp = UGameplayStatics::GetTimeSeconds(this);	// 공격 시작 시각
	}
	else													// 다시 공격 버튼을 눌렀을 때 : 다단 공격 시
	{
		float now = UGameplayStatics::GetTimeSeconds(this);	// 현재 버튼을 누른 시각
		if (now - stamp <= 1.0f && now - stamp >= 0.5f && AttackPhase < 3)	// 0.5초 ~ 1초 사이에 다음 공격 버튼을 눌렀을 경우, 다음 단계의 공격을 진행
		{
			bContinueAttack = true;
			stamp = stamp + 1.0f;	// 다음 공격은 누른 시점과 상관없이 이전 공격의 1초 후
		}
	}
}

void AMainCharacter::CheckEndMovement()
{
	float CurrentPlayerTime;
	if (GetCurrentMovement() == ECustomMovementMode::JUMP)	// JUMP 상태에서의 트랜지션 체크
	{
		CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(41);	// Animation Blueprint에서, 점프 애니메이션 재생 시간
		if (GetCurrentAction() == ECustomActionMode::ATTACK)
		{
			SetCurrentMovement(ECustomMovementMode::IDLE);
		}
		if (CurrentPlayerTime >= AnimationAssetPhases::Jump)	// 점프 시간이 끝남
		{
			if (!bFalling)	// 점프가 끝나 땅에 닿음
			{
				if (MoveKeyPressed > 0)	// 이동키가 눌려있음
				{
					SetCurrentMovement(ECustomMovementMode::WALK);
					FootstepAudioComponent->SetSound(WalkingSound);
					FootstepAudioComponent->SetPitchMultiplier(1.5f);
					FootstepAudioComponent->Play();
				}
				else SetCurrentMovement(ECustomMovementMode::IDLE);
			}
			else SetCurrentMovement(ECustomMovementMode::FALL);	// 땅에 닿지 않았다면, Fall 상태로 전환
		}
	}
	else if (GetCurrentMovement() == ECustomMovementMode::IDLE)	// IDLE 상태에서의 트랜지션 체크
	{
		if (!bForced)	// 외부의 영향을 받지 않은 상태
		{
			if (MoveKeyPressed > 0)	// 이동키가 눌려있음
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
	else if (GetCurrentMovement() == ECustomMovementMode::FALL)	// FALL 상태에서의 트랜지션 체크
	{
		if (bForced)	// 외부의 힘을 받는 상태이면, Idle 애니메이션을 재생
		{
			SetCurrentMovement(ECustomMovementMode::IDLE);
		}
		else if (!bFalling)	// 땅에 닿음
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
		if (bForced || MoveKeyPressed == 0)	// 외부의 힘을 받거나, 이동키를 모두 뗌
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
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(17);	// 펀치 애니메이션의 재생 시간
			if (CurrentPlayerTime >= AnimationAssetPhases::Fist)
			{
				EndAction();
			}
			break;
		case 1:
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(15);	// 발포 애니메이션의 재생 시간
			if (CurrentPlayerTime >= AnimationAssetPhases::Fire)
			{
				EndAction();
			}
			break;
		case 2:
			CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(16);	// 검술 애니메이션의 재생 시간
			if (bAttackBlocked)	// 공격이 막혔으면, 공격을 종료
			{
				EndAction();
				bAttackBlocked = false;
			}
			else if (CurrentPlayerTime >= AnimationAssetPhases::Wield[AttackPhase])	// 현재 단계에서의 재생 시간을 넘김
			{
				if (bContinueAttack)	// 다음 공격 키를 누름
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
		CurrentPlayerTime = GetMesh()->GetAnimInstance()->GetInstanceAssetPlayerTime(21);	// 상호작용 애니메이션의 재생 시간
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
		if (GetCurrentAction() != ECustomActionMode::ATTACK) // 공격 상태가 아닐 때 무기의 히트 판정 ==> 피직스를 끔
		{
			auto Curr = CurrWeapon->ShortForm.NameTag;
			CurrWeapon->MeshComponent->SetSimulatePhysics(false);
			CurrWeapon->MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Curr + "_Equip"));
		}
		else	// 공격 중에 히트 판정 : World Static 오브젝트와 충돌한 경우이므로, 공격 블락 처리
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
			UGameplayStatics::ApplyDamage(Hittee, 30.0f, GetController(), this, NULL);	// 마지막 단계의 공격은 추가 데미지를 입힘
		}
	}
}