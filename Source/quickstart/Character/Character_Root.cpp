// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Root.h"

// Sets default values
ACharacter_Root::ACharacter_Root()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacter_Root::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Root::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HP < 0 && !bDead)
	{
		bDead = true;
		OnDead();
	}

}

// Called to bind functionality to input
void ACharacter_Root::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACharacter_Root::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{

	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HP -= damage;

	OnHurt();

	return damage;
}

void ACharacter_Root::OnHurt()
{
}

void ACharacter_Root::OnDead()
{
}

