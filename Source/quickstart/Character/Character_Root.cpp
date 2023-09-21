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

	if (HP <= 0 && !bDead)	// 체력이 0 이하가 되면 사망 함수 호출
	{
		bDead = true;
		OnDead();
	}

}

float ACharacter_Root::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// 체력을 깎고, Hurt 함수 호출
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HP -= damage;

	OnHurt();

	return damage;
}

void ACharacter_Root::OnHurt()
{
	// 하위 클래스에서 구현
}

void ACharacter_Root::OnDead()
{
	// 하위 클래스에서 구현
}

