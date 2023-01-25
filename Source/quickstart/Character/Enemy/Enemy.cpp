// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "../../Utils/Helpers.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	auto Capsule = Cast<UCapsuleComponent>(RootComponent);
	Capsule->SetCollisionProfileName("Enemy");

	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetCollisionProfileName("NoCollision");
	MainMesh->SetGenerateOverlapEvents(true);

	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	Helpers::SetComponent<UWidgetComponent>(&HPWidget, RootComponent, FVector(0.0f, 0.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HPWidget->SetWidgetClass(HPWidgetClass);
	HPWidget->SetDrawSize(FVector2D(100.0f, 10.0f));
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto HPComp = Cast<UHPBar>(HPWidget->GetWidget());
	if(HPComp) HPComp->HP_ProgressBar->SetPercent(HP / 100.0f);
}

void AEnemy::Attack(int elapsedFrame)
{

}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HP -= Damage;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, ("HP : " + FString::FromInt(HP)));
	return Damage;
}