#include "Character_Root.h"

ACharacter_Root::ACharacter_Root()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacter_Root::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacter_Root::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HP <= 0 && !bDead)	// ü���� 0 ���ϰ� �Ǹ� ��� �Լ� ȣ��
	{
		bDead = true;
		OnDead();
	}

}

float ACharacter_Root::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// ü���� ���, Hurt �Լ� ȣ��
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HP -= damage;

	OnHurt();

	return damage;
}

void ACharacter_Root::OnHurt()
{
	// ���� Ŭ�������� ����
}

void ACharacter_Root::OnDead()
{
	// ���� Ŭ�������� ����
}

